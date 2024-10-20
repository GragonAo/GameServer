#include <iostream>

#include "common.h"
#include "network_connector.h"
#include "network_locator.h"
#include "log4_help.h"
#include "yaml.h"
#include "thread_mgr.h"
#include "update_component.h"
#include "component_help.h"
#include "message_callback.h"
#include "message_system.h"

// 初始化网络连接器
void NetworkConnector::Awake(int iType, int mixConnectAppType)
{
    // 将传入的 iType 转换为 NetworkType 枚举并保存
    _networkType = (NetworkType)iType;

    // 获取 NetworkLocator 组件，并将当前 NetworkConnector 注册到 NetworkLocator 中
    auto pNetworkLocator = ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
    pNetworkLocator->AddConnectorLocator(this, _networkType);

    // 添加 UpdateComponent，用于更新网络连接状态
    AddComponent<UpdateComponent>(BindFunP0(this, &NetworkConnector::Update));

    // 添加 MessageComponent，并注册处理网络连接的消息回调函数
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_NetworkConnect, BindFunP1(this, &NetworkConnector::HandleNetworkConnect));
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_NetworkRequestDisconnect, BindFunP1(this, &NetworkConnector::HandleDisconnect));

#ifdef EPOLL
    std::cout << "epoll model. connector:" << GetNetworkTypeName(_networkType) << std::endl;
    InitEpoll();  // 如果使用 EPOLL 模式，初始化 EPOLL
#else
    std::cout << "select model. connector:" << GetNetworkTypeName(_networkType) << std::endl;
#endif

    // 如果当前是 TCP 连接器且需要连接的应用类型大于 0
    if (_networkType == NetworkType::TcpConnector && mixConnectAppType > 0)
    {
        // 使用 YAML 配置文件获取各个应用类型的 IP 地址和端口，并创建连接器
        const auto pYaml = ComponentHelp::GetYaml();
        APP_TYPE appType;
        
        // 判断是否需要连接 APP_APPMGR 类型的应用
        if ((mixConnectAppType & APP_APPMGR) != 0)
        {
            appType = APP_APPMGR;
            const auto pCommonConfig = pYaml->GetIPEndPoint(appType, 0);
            CreateConnector(appType, 0, pCommonConfig->Ip, pCommonConfig->Port);
        }

        // 判断是否需要连接 APP_DB_MGR 类型的应用
        if ((mixConnectAppType & APP_DB_MGR) != 0)
        {
            appType = APP_DB_MGR;
            const auto pCommonConfig = pYaml->GetIPEndPoint(appType, 0);
            CreateConnector(appType, 0, pCommonConfig->Ip, pCommonConfig->Port);
        }

        // 判断是否需要连接 APP_LOGIN 类型的应用
        if ((mixConnectAppType & APP_LOGIN) != 0)
        {
            appType = APP_LOGIN;
            auto pLoginConfig = dynamic_cast<LoginConfig*>(pYaml->GetConfig(appType));
            for (auto one : pLoginConfig->Apps)
            {
                CreateConnector(appType, one.Id, one.Ip, one.Port);
            }
        }

        // 判断是否需要连接 APP_SPACE 类型的应用
        if ((mixConnectAppType & APP_SPACE) != 0)
        {
            appType = APP_SPACE;
            auto pLoginConfig = dynamic_cast<SpaceConfig*>(pYaml->GetConfig(appType));
            for (auto one : pLoginConfig->Apps)
            {
                CreateConnector(appType, one.Id, one.Ip, one.Port);
            }
        }
    }
}

// 创建一个连接器
void NetworkConnector::CreateConnector(APP_TYPE appType, int appId, std::string ip, int port)
{
    // 创建连接详细信息并加入待连接的缓存中
    ConnectDetail* pDetail = new ConnectDetail(
        { ObjectKeyType::App, { GetAppKey(appType, appId), ""} }, 
        ip, port);
    _connecting.AddObj(pDetail);
}

// 获取类型名称
const char* NetworkConnector::GetTypeName()
{
    return typeid(NetworkConnector).name();
}

// 获取类型的哈希值
uint64 NetworkConnector::GetTypeHashCode()
{
    return typeid(NetworkConnector).hash_code();
}

// 尝试与指定的连接详细信息进行连接
bool NetworkConnector::Connect(ConnectDetail* pDetail)
{
    const int socket = CreateSocket();  // 创建 socket
    if (socket == INVALID_SOCKET)
        return false;

    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(pDetail->Port);  // 设置端口号
    ::inet_pton(AF_INET, pDetail->Ip.c_str(), &addr.sin_addr.s_addr);  // 设置 IP 地址

    const int rs = ::connect(socket, (struct sockaddr*)&addr, sizeof(sockaddr));  // 尝试连接
    if (rs == 0)
    {
        return CreateConnectObj(socket, pDetail->Key, ConnectStateType::Connected);  // 如果连接成功，创建连接对象
    }
    else
    {
        const auto socketError = _sock_err();  // 获取 socket 错误信息
        if (!NetworkHelp::IsError(socketError))
        {
            // 如果不是错误，表示连接正在进行
#ifdef LOG_TRACE_COMPONENT_OPEN
            std::stringstream traceMsg;
            traceMsg << "create connect != 0 waiting, err=" << socketError;
            traceMsg << " network type:" << GetNetworkTypeName(_networkType);
            ComponentHelp::GetTraceComponent()->Trace(TraceType::Connector, socket, traceMsg.str());
#endif
            return CreateConnectObj(socket, pDetail->Key, ConnectStateType::Connecting);  // 创建正在连接的对象
        }
        else
        {
            // 如果是错误，记录日志并关闭 socket
            LOG_WARN("failed to connect 2. ip:" << pDetail->Ip.c_str() << " port:" << pDetail->Port << " network sn:" << _sn << " socket:" << socket << " err:" << socketError);
            _sock_close(socket);
            return false;
        }
    }
}

// 处理网络连接的回调函数
void NetworkConnector::HandleNetworkConnect(Packet* pPacket)
{
    auto proto = pPacket->ParseToProto<Proto::NetworkConnect>();
    if (proto.network_type() != (int)_networkType)
        return;

    ObjectKey key;
    key.ParseFromProto(proto.key());
    ConnectDetail* pDetail = new ConnectDetail(key, proto.ip(), proto.port());
    _connecting.AddObj(pDetail);
}

#ifdef EPOLL

// 定期更新，处理正在连接的对象并执行 EPOLL 操作
void NetworkConnector::Update()
{
    if (_connecting.CanSwap())  // 检查是否可以交换缓存
        _connecting.Swap(nullptr);

    if (!_connecting.GetReaderCache()->empty())
    {
        auto pReader = _connecting.GetReaderCache();
        for (auto iter = pReader->begin(); iter != pReader->end(); ++iter)
        {
            if (Connect(iter->second))  // 尝试连接
            {
                _connecting.RemoveObj(iter->first);  // 连接成功后移除缓存中的对象
            }
        }
    }

    Epoll();  // 处理 EPOLL 事件
    OnNetworkUpdate();
}

#else

// 使用 Select 模型进行更新
void NetworkConnector::Update()
{
    if (_connecting.CanSwap())  // 检查是否可以交换缓存
        _connecting.Swap(nullptr);

    if (!_connecting.GetReaderCache()->empty())
    {
        auto pReader = _connecting.GetReaderCache();
        for (auto iter = pReader->begin(); iter != pReader->end(); ++iter)
        {
            if (Connect(iter->second))  // 尝试连接
            {
                _connecting.RemoveObj(iter->first);  // 连接成功后移除缓存中的对象
            }
        }
    }

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    _fdMax = 0;

    Select();  // 处理 Select 模型的 IO 事件
    OnNetworkUpdate();
}

#endif
