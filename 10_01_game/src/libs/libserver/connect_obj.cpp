#include "connect_obj.h"

#include "network.h"             // 引入网络相关定义
#include "network_buffer.h"       // 引入网络缓冲区定义

#include "thread_mgr.h"           // 引入线程管理器
#include "system_manager.h"       // 引入系统管理器
#include "message_system.h"       // 引入消息系统
#include "message_system_help.h"  // 引入消息系统辅助类
#include "component_help.h"       // 引入组件帮助类

#include "object_pool_packet.h"   // 引入对象池数据包
#include "global.h"               // 引入全局定义
#include "network_locator.h"      // 引入网络定位器

// 构造函数，初始化连接状态和接收/发送缓冲区
ConnectObj::ConnectObj()
{
    _state = ConnectStateType::None;   // 初始化连接状态为 None
    _recvBuffer = new RecvNetworkBuffer(DEFAULT_RECV_BUFFER_SIZE, this); // 创建接收缓冲区
    _sendBuffer = new SendNetworkBuffer(DEFAULT_SEND_BUFFER_SIZE, this); // 创建发送缓冲区
}

// 析构函数，释放接收和发送缓冲区
ConnectObj::~ConnectObj()
{
    delete _recvBuffer;  // 释放接收缓冲区
    delete _sendBuffer;  // 释放发送缓冲区
}

// 通过Socket、网络类型、对象键和状态来唤醒连接对象
void ConnectObj::Awake(SOCKET socket, NetworkType networkType, ObjectKey key, ConnectStateType state)
{
    _socketKey = SocketKey(socket, networkType); // 初始化Socket键
    _objKey = key;  // 设置对象键
    _state = state; // 设置连接状态
}

// 将连接对象放回对象池，并清理资源
void ConnectObj::BackToPool()
{
#ifdef LOG_TRACE_COMPONENT_OPEN
    // 如果打开了日志跟踪，则记录连接关闭的日志
    const auto traceMsg = std::string("close.  network type:").append(GetNetworkTypeName(_socketKey.NetType));
    ComponentHelp::GetTraceComponent()->Trace(TraceType::Connector, _socketKey.Socket, traceMsg);
#endif

    if (GetObjectKey().KeyType == ObjectKeyType::App)
    {
        // 如果对象键是应用程序类型，移除网络标识
        auto pLocator = ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
        const auto key = GetObjectKey().KeyValue.KeyInt64;
        pLocator->RemoveNetworkIdentify(key);
    }
    else
    {
        // 通知逻辑层，连接关闭
        MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkDisconnect, this);
    }

    // 如果Socket有效，关闭Socket
    if (_socketKey.Socket != INVALID_SOCKET)
        _sock_close(_socketKey.Socket);

    // 重置连接状态和相关键
    _state = ConnectStateType::None;
    _socketKey.Clean();
    _objKey.Clean();

    // 将接收和发送缓冲区放回对象池
    _recvBuffer->BackToPool();
    _sendBuffer->BackToPool();
}

// 检查接收缓冲区是否有数据
bool ConnectObj::HasRecvData() const
{
    return _recvBuffer->HasData();
}

// 接收网络数据并处理
bool ConnectObj::Recv()
{
    if (_state == ConnectStateType::Connecting)
    {
        // 如果当前处于连接状态，将状态改为已连接
        ChangeStateToConnected();
        return true;
    }

    bool isRs = false;
    char* pBuffer = nullptr;
    
    while (true)
    {
        // 如果接收缓冲区空间不足，重新分配缓冲区
        if (_recvBuffer->GetEmptySize() < (sizeof(PacketHead) + sizeof(TotalSizeType)))
        {
            _recvBuffer->ReAllocBuffer();
        }

        // 获取接收缓冲区指针，并读取数据
        const int emptySize = _recvBuffer->GetBuffer(pBuffer);
        const int dataSize = ::recv(_socketKey.Socket, pBuffer, emptySize, 0);
        
        if (dataSize > 0)
        {
            // 如果有数据，将数据填充到缓冲区
            _recvBuffer->FillDate(dataSize);
            isRs = true;
        }
        else if (dataSize == 0)
        {
            // 没有数据，结束接收
            break;
        }
        else
        {
            // 处理错误情况
            const auto socketError = _sock_err();
            isRs = socketError != 0;
            if (!NetworkHelp::IsError(socketError))
            {
                isRs = true;
            }

            if (!isRs)
                LOG_WARN("recv size:" << dataSize << " error:" << socketError);

            break;
        }
    }

    if (isRs)
    {
        // 获取网络类型并处理接收到的数据包
        const auto pNetwork = this->GetParent<Network>();
        const auto iNetworkType = pNetwork->GetNetworkType();
        
        while (true)
        {
            const auto pPacket = _recvBuffer->GetPacket();
            if (pPacket == nullptr)
                break;

            const auto msgId = pPacket->GetMsgId();
            const bool isTcp = NetworkHelp::IsTcp(iNetworkType);
            
            if (!isTcp && msgId == Proto::MsgId::MI_HttpRequestBad)
            {
                // 如果是HTTP请求错误，发送404响应
                MessageSystemHelp::SendHttpResponse404(pPacket);
                DynamicPacketPool::GetInstance()->FreeObject(pPacket);
                continue;
            }

            if (isTcp && msgId == Proto::MsgId::MI_Ping)
            {
                // 如果是TCP的Ping消息，忽略处理
                continue;
            }

            if (!isTcp && (msgId <= Proto::MsgId::MI_HttpBegin || msgId >= Proto::MsgId::MI_HttpEnd) && msgId != Proto::MsgId::MI_HttpOuterResponse)
            {
                // 非TCP协议接收到错误的HTTP消息
                LOG_WARN("http connect recv. tcp proto");
                DynamicPacketPool::GetInstance()->FreeObject(pPacket);
                continue;
            }

            // 将接收到的数据包分发到线程管理器处理
            ThreadMgr::GetInstance()->DispatchPacket(pPacket);
        }
    }

    return isRs;
}

// 检查发送缓冲区是否有数据
bool ConnectObj::HasSendData() const
{
    return _sendBuffer->HasData();
}

// 将Packet添加到发送缓冲区
void ConnectObj::SendPacket(Packet* pPacket) const
{
    _sendBuffer->AddPacket(pPacket);    // 添加数据包到发送缓冲区
    DynamicPacketPool::GetInstance()->FreeObject(pPacket);  // 释放数据包
}

// 发送网络数据
bool ConnectObj::Send()
{
    if (_state == ConnectStateType::Connecting)
    {
        // 如果当前处于连接状态，将状态改为已连接
        ChangeStateToConnected();
        return true;
    }

    while (true) {
        char* pBuffer = nullptr;
        const int needSendSize = _sendBuffer->GetBuffer(pBuffer);

        // 如果没有数据可以发送，返回 true
        if (needSendSize <= 0)
        {
            return true;
        }

        const int size = ::send(_socketKey.Socket, pBuffer, needSendSize, 0);
        if (size > 0)
        {
            // 发送成功后移除已发送的数据
            _sendBuffer->RemoveDate(size);

            // 如果没有全部发送完，则继续发送
            if (size < needSendSize)
            {
                return true;
            }
        }

        if (size <= 0)
        {
            // 处理发送错误
            const auto socketError = _sock_err();
            std::cout << "needSendSize:" << needSendSize << " error:" << socketError << std::endl;
            return false;
        }
    }
}

// 关闭连接，并通知系统
void ConnectObj::Close()
{
    const auto pPacketDis = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_NetworkRequestDisconnect, this);
    GetSystemManager()->GetMessageSystem()->AddPacketToList(pPacketDis);
    pPacketDis->OpenRef();  // 打开引用计数
}

// 获取连接状态
ConnectStateType ConnectObj::GetState() const
{
    return _state;
}

// 将连接状态改为已连接
void ConnectObj::ChangeStateToConnected()
{
    _state = ConnectStateType::Connected;
    
    if (GetObjectKey().KeyType == ObjectKeyType::App)
    {
        // 如果对象键是应用程序类型，添加网络标识
        auto pLocator = ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
        pLocator->AddNetworkIdentify(GetSocketKey(), GetObjectKey());
    }
    else
    {
        // 通知逻辑层，连接成功
        MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkConnected, this);
    }
}

// 修改连接对象的键
void ConnectObj::ModifyConnectKey(ObjectKey key)
{
    _objKey = key;   // 更新对象键
}
