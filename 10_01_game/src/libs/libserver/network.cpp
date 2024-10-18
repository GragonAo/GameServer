#include "network.h"
#include "connect_obj.h"
#include "packet.h"
#include "common.h"
#include "object_pool.h"
#include <iostream>
#include "component_help.h"
#include "object_pool_packet.h"

// 将当前 Network 对象恢复到对象池中，清除所有连接对象和发送消息队列
void Network::BackToPool()
{
    // 清空待发送消息列表
    _sendMsgList.BackToPool();

    // 遍历所有连接对象，将它们返回对象池
    for (auto iter = _connects.begin(); iter != _connects.end(); ++iter)
    {
        auto pObj = iter->second;
        pObj->ComponentBackToPool();  // 释放连接对象的资源
    }
    _connects.clear();  // 清空连接列表

#ifdef EPOLL
    // 如果使用 epoll，关闭 epoll 文件描述符
    ::close(_epfd);
    _epfd = -1;
#endif
}

// 设置 socket 选项，如超时、keep-alive 等
void Network::SetSocketOpt(SOCKET socket)
{
    // 1. 设置发送和接收的超时时间为 10000 毫秒
    int netTimeout = 10000;  // 1000 毫秒 = 1 秒
    setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (SetsockOptType)&netTimeout, sizeof(netTimeout));
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (SetsockOptType)&netTimeout, sizeof(netTimeout));

#if ENGINE_PLATFORM != PLATFORM_WIN32

    // 如果不是 Http 连接或监听，设置 keep-alive 选项
    if (_networkType != NetworkType::HttpConnector && _networkType != NetworkType::HttpListen)
    {
        int keepAlive = 1;
        socklen_t optlen = sizeof(keepAlive);

        int keepIdle = 60 * 2;  // 没有数据传输的空闲时间后开始发送 keep-alive 包
        int keepInterval = 10;  // 每次发送 keep-alive 包的时间间隔
        int keepCount = 5;      // 连续发送 keep-alive 包的次数

        // 启用 keep-alive 选项
        setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (SetsockOptType)&keepAlive, optlen);
        if (getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, &optlen) < 0)
        {
            LOG_WARN("getsockopt SO_KEEPALIVE failed. err:" << _sock_err() << " socket:" << socket << " networktype:" << GetNetworkTypeName(_networkType));
        }

        // 设置 keep-alive 相关参数
        setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, optlen);
        setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void*)&keepInterval, optlen);
        setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void*)&keepCount, optlen);
    }

#endif

    // 将 socket 设置为非阻塞模式
    _sock_nonblock(socket);
}

// 创建并返回一个新的 socket，根据网络类型区分 Http 或 TCP
SOCKET Network::CreateSocket()
{
    _sock_init();  // 初始化 socket 环境（例如在 Windows 上进行 WSAStartup）
    SOCKET socket;

    // 如果是 Http 网络类型，使用 IPPROTO_IP 协议，否则使用 IPPROTO_TCP
    if (_networkType == NetworkType::HttpListen || _networkType == NetworkType::HttpConnector)
        socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    else
        socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 如果 socket 创建失败，输出错误信息
    if (socket == INVALID_SOCKET)
    {
        std::cout << "::socket failed. err:" << _sock_err() << std::endl;
        return socket;
    }

    // 设置 socket 选项
    SetSocketOpt(socket);
    return socket;
}

// 检查一个 socket 是否有效，如果无效则关闭它
bool Network::CheckSocket(SOCKET socket)
{
    int err = EBADF;  // 初始化错误代码
    socklen_t len = sizeof(err);

    // 获取 socket 错误状态，确认是否有错误
    if (::getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)(&err), &len) == 0)
    {
        if (!NetworkHelp::IsError(err))
        {
            err = 0;  // 没有错误
        }
    }

    // 如果有错误，关闭 socket 并返回 false
    if (err != 0)
    {
        _sock_close(socket);  // 关闭 socket
        return false;
    }

    return true;  // socket 有效
}

// 创建一个新的连接对象，并将其加入连接列表
bool Network::CreateConnectObj(SOCKET socket, ObjectKey key, ConnectStateType iState)
{
    // 首先检查 socket 是否有效
    if (!CheckSocket(socket))
        return false;

    // 检查该 socket 是否已存在连接对象，如果已存在则移除
    const auto iter = _connects.find(socket);
    if (iter != _connects.end())
    {
        LOG_ERROR("Network::CreateConnectObj. socket is exist. socket:" << socket << " sn:" << _sn);
        _connects[socket]->ComponentBackToPool();  // 释放已存在的连接对象资源
        _connects.erase(iter);  // 从连接列表中删除
        return false;
    }

    // 创建一个新的 ConnectObj 对象，并将其加入连接列表
    auto pCollector = _pSystemManager->GetPoolCollector();
    auto pPool = dynamic_cast<DynamicObjectPool<ConnectObj>*>(pCollector->GetPool<ConnectObj>());
    ConnectObj* pConnectObj = pPool->MallocObject(_pSystemManager, socket, _networkType, key, iState);
    pConnectObj->SetParent(this);

    _connects[socket] = pConnectObj;

#ifdef LOG_TRACE_COMPONENT_OPEN
    const auto traceMsg = std::string("create. network type:").append(GetNetworkTypeName(_networkType));
    ComponentHelp::GetTraceComponent()->Trace(TraceType::Connector, socket, traceMsg);
#endif

#ifdef EPOLL
    // 如果使用 epoll，注册 socket 事件
    AddEvent(_epfd, socket, EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP);
#endif

    return true;
}

// 处理断开连接的事件，移除相应的连接对象
void Network::HandleDisconnect(Packet* pPacket)
{
    const auto socketKey = pPacket->GetSocketKey();
    if (socketKey.NetType != _networkType)
        return;  // 如果网络类型不匹配，则返回

    // 查找并移除对应的连接对象
    auto iter = _connects.find(socketKey.Socket);
    if (iter == _connects.end())
    {
        std::cout << "dis connect failed. socket not find." << pPacket << std::endl;
        return;
    }

    RemoveConnectObj(iter);  // 移除连接对象
}

#ifdef EPOLL

// 在 epoll 中添加事件监听
void Network::AddEvent(int epollfd, int fd, int flag)
{
    struct epoll_event ev;
    ev.events = flag;
    ev.data.ptr = nullptr;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);  // 添加事件到 epoll
}

// 修改 epoll 中的事件
void Network::ModifyEvent(int epollfd, int fd, int flag)
{
    struct epoll_event ev;
    ev.events = flag;
    ev.data.ptr = nullptr;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);  // 修改事件
}

// 删除 epoll 中的事件
void Network::DeleteEvent(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);  // 从 epoll 中删除事件
}

// 初始化 epoll
void Network::InitEpoll()
{
    _epfd = epoll_create(MAX_CLIENT);  // 创建 epoll 文件描述符
}

// 处理 epoll 事件循环
void Network::Epoll()
{
    const int nfds = epoll_wait(_epfd, _events, MAX_EVENT, 0);  // 等待事件触发
    for (int index = 0; index < nfds; index++)
    {
        SOCKET fd = _events[index].data.fd;
        OnEpoll(fd, index);  // 处理 epoll 事件

        auto iter = _connects.find(fd);
        if (iter == _connects.end())
            continue;

        // 处理不同的事件类型，如断开连接、读写操作
        if (_events[index].events & EPOLLRDHUP || _events[index].events & EPOLLERR || _events[index].events & EPOLLHUP)
        {
            RemoveConnectObj(iter);
            continue;
        }

        if (_events[index].events & EPOLLIN)
        {
            if (!iter->second->Recv())
            {
                RemoveConnectObj(iter);
                continue;
            }
        }

        if (_events[index].events & EPOLLOUT)
        {
            if (!iter->second->Send())
            {
                RemoveConnectObj(iter);
                continue;
            }

            ModifyEvent(_epfd, iter->first, EPOLLIN | EPOLLRDHUP);
        }
    }
}

#else

// 通过 select 进行事件监听（如果不使用 epoll）
void Network::Select()
{
    // 初始化文件描述符集合
    fd_set readfds, writefds, exceptfds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    // 遍历所有连接对象，将其添加到文件描述符集合中
    for (auto iter = _connects.begin(); iter != _connects.end(); ++iter)
    {
        SOCKET socket = iter->first;
        FD_SET(socket, &readfds);   // 添加到读文件描述符集合
        FD_SET(socket, &writefds);  // 添加到写文件描述符集合
        FD_SET(socket, &exceptfds); // 添加到异常文件描述符集合
    }

    struct timeval timeout = {0, 0};  // 设置超时时间为 0，立即返回
    select(0, &readfds, &writefds, &exceptfds, &timeout);  // 调用 select 进行事件监听

    // 处理每个 socket 的事件
    for (auto iter = _connects.begin(); iter != _connects.end(); ++iter)
    {
        auto pObj = iter->second;

        if (FD_ISSET(iter->first, &readfds))
        {
            // 处理读事件
            if (!pObj->Recv())
            {
                RemoveConnectObj(iter);
                continue;
            }
        }

        if (FD_ISSET(iter->first, &writefds))
        {
            // 处理写事件
            if (!pObj->Send())
            {
                RemoveConnectObj(iter);
                continue;
            }
        }

        if (FD_ISSET(iter->first, &exceptfds))
        {
            // 处理异常事件（如断开连接）
            std::cout << "socket except!! socket:" << iter->first << std::endl;
            RemoveConnectObj(iter);
            continue;
        }
    }
}
#endif


// 定期更新网络状态，处理发送队列中的数据包并将其发送至对应的连接对象
void Network::OnNetworkUpdate()
{
    // 锁定发送消息的互斥量，确保线程安全
    _sendMsgMutex.lock();

    // 检查待发送列表是否可以交换，如果可以则进行读写缓存的交换
    if (_sendMsgList.CanSwap())
    {
        _sendMsgList.Swap();  // 交换读写缓存，便于接下来的发送操作
    }

    // 解锁互斥量，允许其他线程访问发送队列
    _sendMsgMutex.unlock();

    // 获取发送消息队列的读取缓存
    auto pList = _sendMsgList.GetReaderCache();

    // 遍历读取缓存中的每个数据包，逐一处理
    for (auto iter = pList->begin(); iter != pList->end(); ++iter)
    {
        Packet* pPacket = (*iter);  // 获取当前要发送的数据包
        auto socketKey = pPacket->GetSocketKey();  // 获取数据包中的 socket 相关信息

        // 查找对应的连接对象
        auto iterObj = _connects.find(socketKey.Socket);
        if (iterObj == _connects.end())
        {
            // 如果找不到对应的连接对象，输出错误日志并释放数据包
            LOG_ERROR("failed to send packet. can't find socket." << pPacket);
            DynamicPacketPool::GetInstance()->FreeObject(pPacket);  // 释放数据包内存
            continue;  // 跳过当前数据包，处理下一个
        }

        // 获取连接对象
        const auto pObj = iterObj->second;

        // 验证数据包的对象键是否与连接对象的键一致
        if (pObj->GetObjectKey() != pPacket->GetObjectKey())
        {
            // 如果不一致，输出错误日志并释放数据包
            LOG_ERROR("failed to send packet. connect key is different. packet[" << pPacket << "] connect:[" << pObj << "]");
            DynamicPacketPool::GetInstance()->FreeObject(pPacket);  // 释放数据包内存
            continue;  // 跳过当前数据包，处理下一个
        }

        // 调用连接对象的发送函数，发送数据包
        pObj->SendPacket(pPacket);

#ifdef EPOLL
        // 如果使用 epoll，将 socket 事件修改为读写、断开检测等事件
        ModifyEvent(_epfd, socketKey.Socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
#endif
    }

    // 清空读取缓存，准备下一次交换
    pList->clear();
}

// 将数据包添加到发送队列中，并等待在合适时机发送
void Network::SendPacket(Packet*& pPacket)
{
    // 使用 std::lock_guard 自动管理互斥量锁定，确保线程安全
    std::lock_guard<std::mutex> guard(_sendMsgMutex);

    // 检查数据包的网络类型是否与当前 Network 对象的网络类型一致
    if (pPacket->GetSocketKey().NetType != _networkType)
    {
        // 如果网络类型不一致，输出错误日志并直接返回
        LOG_ERROR("failed to send packet. network type is different." << pPacket);
        return;
    }

    // 将数据包添加到写缓存中，等待后续的发送处理
    _sendMsgList.GetWriterCache()->emplace_back(pPacket);

#ifdef LOG_TRACE_COMPONENT_OPEN
    // 如果打开了组件的追踪日志功能，记录发送的数据包信息
    const google::protobuf::EnumDescriptor* descriptor = Proto::MsgId_descriptor();
    const auto name = descriptor->FindValueByNumber(pPacket->GetMsgId())->name();  // 获取消息 ID 的名称

    // 生成追踪消息内容，包括包的序列号、消息 ID 等信息
    const auto traceMsg = std::string("send net.")
        .append(" sn:").append(std::to_string(pPacket->GetSN()))
        .append(" msgId:").append(name);

    // 调用追踪组件，记录该数据包的发送情况
    ComponentHelp::GetTraceComponent()->Trace(TraceType::Packet, pPacket->GetSocketKey().Socket, traceMsg);
#endif
}
