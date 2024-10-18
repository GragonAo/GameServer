#include <iostream>
#include <set>

#include "common.h"
#include "network_listen.h"
#include "connect_obj.h"
#include "thread_mgr.h"
#include "network_locator.h"
#include "log4_help.h"
#include "message_component.h"
#include "update_component.h"
#include "component_help.h"
#include "global.h"

// 初始化网络监听器，绑定 IP、端口和网络类型
void NetworkListen::Awake(std::string ip, int port, NetworkType iType)
{
    _networkType = iType;

    // 获取 NetworkLocator 实例，并注册当前监听器
    auto pNetworkLocator = ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
    pNetworkLocator->AddListenLocator(this, iType);

    // 注册消息回调组件，用于处理特定的网络消息
    auto pMsgCallBack = new MessageCallBackFunction();
    AddComponent<MessageComponent>(pMsgCallBack);
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_NetworkRequestDisconnect, BindFunP1(this, &NetworkListen::HandleDisconnect));
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_NetworkListenKey, BindFunP1(this, &NetworkListen::HandleListenKey));

    // 添加更新组件，用于定期执行更新逻辑
    auto pUpdateComponent = AddComponent<UpdateComponent>();
    pUpdateComponent->UpdataFunction = BindFunP0(this, &NetworkListen::Update);

    // 创建主 socket，用于监听客户端连接
    _masterSocket = CreateSocket();
    if (_masterSocket == INVALID_SOCKET)
        return;

    // 允许端口重用，以便更灵活地绑定端口
    int isOn = 1;
    setsockopt(_masterSocket, SOL_SOCKET, SO_REUSEADDR, (SetsockOptType)&isOn, sizeof(isOn));

    // 绑定 IP 和端口
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);  // 将端口号转换为网络字节序
    ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);  // 将 IP 地址转换为二进制格式

    // 绑定失败时输出错误信息
    if (::bind(_masterSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        LOG_ERROR("::bind failed. err:" << _sock_err());
        return;
    }

    // 开始监听连接，最大连接数为 1024
    const int maxConn = 1024;  // 最大连接数
    if (::listen(_masterSocket, maxConn) < 0)
    {
        std::cout << "::listen failed." << _sock_err() << std::endl;
        return;
    }

#ifdef EPOLL
    LOG_INFO("epoll model. listen " << ip.c_str() << ":" << port << " SOMAXCONN:" << maxConn);
    InitEpoll();
    AddEvent(_epfd, _masterSocket, EPOLLIN | EPOLLET | EPOLLOUT | EPOLLRDHUP);
#else
    LOG_INFO("select model. listen " << ip.c_str() << ":" << port << " SOMAXCONN:" << maxConn);
#endif

    return;
}

// 将网络监听器重置并释放资源
void NetworkListen::BackToPool()
{
#ifdef EPOLL
    _mainSocketEventIndex = -1;  // 重置主 socket 事件索引
#endif

    // 关闭主 socket 并重置为无效状态
    _sock_close(_masterSocket);
    _masterSocket = INVALID_SOCKET;

    Network::BackToPool();  // 调用基类的回收函数
}

// 通过应用类型和应用 ID 初始化监听器，读取配置文件获取端口和 IP
void NetworkListen::Awake(int appType, int appId)
{
    auto pGlobal = Global::GetInstance();
    auto pYaml = ComponentHelp::GetYaml();
    const auto pCommonConfig = pYaml->GetIPEndPoint(pGlobal->GetCurAppType(), pGlobal->GetCurAppId());

    if (pCommonConfig == nullptr)
    {
        LOG_ERROR("failed to get config of listen. appType:" << GetAppName(pGlobal->GetCurAppType()) << " appId:" << pGlobal->GetCurAppId());
        return;
    }

    // 使用配置中的 IP 和端口初始化
    Awake(pCommonConfig->Ip, pCommonConfig->Port, NetworkType::TcpListen);
}

// 通过 IP 和端口初始化 HTTP 监听器
void NetworkListen::Awake(std::string ip, int port)
{
    Awake(ip, port, NetworkType::HttpListen);
}

// 接受新的客户端连接
int NetworkListen::Accept()
{
    struct sockaddr socketClient;
    socklen_t socketLength = sizeof(socketClient);

    int rs = 0;  // 记录成功接受的连接数
    while (true)
    {
        const SOCKET socket = ::accept(_masterSocket, &socketClient, &socketLength);  // 接受连接
        if (socket == INVALID_SOCKET)
            return rs;

        // 创建连接对象，处理新连接
        if (!CreateConnectObj(socket, ObjectKey(), ConnectStateType::Connected))
        {
            _sock_close(socket);  // 如果创建失败，关闭 socket
            continue;
        }

        SetSocketOpt(socket);  // 设置 socket 选项
        ++rs;  // 记录成功连接数
    }
}

// 返回当前类的类型名称，用于调试
const char* NetworkListen::GetTypeName()
{
    return typeid(NetworkListen).name();
}

// 返回当前类的哈希码，用于类型识别
uint64 NetworkListen::GetTypeHashCode()
{
    return typeid(NetworkListen).hash_code();
}

// 打印当前监听器的 socket 数量，用于调试
void NetworkListen::CmdShow()
{
    LOG_DEBUG("\tsocket size:" << _connects.size());
}

#ifndef EPOLL

// 使用 select 模型更新监听状态，处理新连接
void NetworkListen::Update()
{
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    FD_SET(_masterSocket, &readfds);
    FD_SET(_masterSocket, &writefds);
    FD_SET(_masterSocket, &exceptfds);

    _fdMax = _masterSocket;  // 设置最大文件描述符

    Select();  // 执行 select 系统调用

    // 如果有新的连接请求，接受连接
    if (FD_ISSET(_masterSocket, &readfds))
    {
        Accept();
    }

    Network::OnNetworkUpdate();  // 更新网络状态
}

#else

// 在 epoll 模型下，处理 epoll 事件
void NetworkListen::OnEpoll(SOCKET fd, int index)
{
    if (fd == _masterSocket)
    {
        _mainSocketEventIndex = index;  // 记录主 socket 的事件索引
    }
}

// 使用 epoll 模型更新监听状态，处理新连接
void NetworkListen::Update()
{
    _mainSocketEventIndex = -1;

    Epoll();  // 执行 epoll 系统调用

    // 如果有新的连接请求，接受连接
    if (_mainSocketEventIndex >= 0)
    {
        Accept();
    }

    Network::OnNetworkUpdate();  // 更新网络状态
}

#endif

// 处理网络监听键的回调函数，修改连接对象的键值
void NetworkListen::HandleListenKey(Packet* pPacket)
{
    const auto socketKey = pPacket->GetSocketKey();
    if (socketKey.NetType != _networkType)
        return;

    // 查找对应的连接对象
    auto iter = _connects.find(socketKey.Socket);
    if (iter == _connects.end())
    {
        std::cout << "failed to modify connect key. socket not find." << pPacket << std::endl;
        return;
    }

    // 修改连接对象的键值
    auto pObj = iter->second;
    pObj->ModifyConnectKey(pPacket->GetObjectKey());
}
