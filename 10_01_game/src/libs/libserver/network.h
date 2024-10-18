#pragma once

#include <map>
#include "common.h"
#include "entity.h"
#include "cache_swap.h"
#include "network_type.h"
#include "network_help.h"
#include "connect_obj.h"

#if ENGINE_PLATFORM != PLATFORM_WIN32
// 非 Windows 平台的头文件和宏定义
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#ifdef EPOLL
#include <sys/epoll.h>
#endif

// 套接字相关的宏定义，用于简化跨平台代码
#define _sock_init( )               // 非 Windows 平台下套接字初始化为空
#define _sock_nonblock( sockfd ) { int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); } // 设置非阻塞模式
#define _sock_exit( )               // 非 Windows 平台下的套接字清理为空
#define _sock_err( )	errno         // 获取套接字错误码
#define _sock_close( sockfd ) ::close( sockfd ) // 关闭套接字
#define _sock_is_blocked()	(errno == EAGAIN || errno == 0) // 判断是否是非阻塞错误

#ifdef EPOLL
// 移除连接对象的宏定义
#define RemoveConnectObj(iter) \
    iter->second->ComponentBackToPool( ); \
    DeleteEvent(_epfd, iter->first); \
    iter = _connects.erase( iter ); 
#else
// 移除连接对象的宏定义
#define RemoveConnectObj(iter) \
    iter->second->ComponentBackToPool( ); \
    iter = _connects.erase( iter ); 
#endif


#else

// Windows 平台下的套接字相关宏定义
#define _sock_init( )	{ WSADATA wsaData; WSAStartup( MAKEWORD(2, 2), &wsaData ); } // 初始化 Winsock
#define _sock_nonblock( sockfd )	{ unsigned long param = 1; ioctlsocket(sockfd, FIONBIO, (unsigned long *)&param); } // 设置非阻塞模式
#define _sock_exit( )	{ WSACleanup(); } // 清理 Winsock
#define _sock_err( )	WSAGetLastError() // 获取套接字错误码
#define _sock_close( sockfd ) ::closesocket( sockfd ) // 关闭套接字
#define _sock_is_blocked()	(WSAGetLastError() == WSAEWOULDBLOCK) // 判断是否是非阻塞错误

// 移除连接对象的宏定义
#define RemoveConnectObj(iter) \
    iter->second->ComponentBackToPool( ); \
    iter = _connects.erase( iter ); 

#endif

#if ENGINE_PLATFORM != PLATFORM_WIN32
#define SetsockOptType void *  // 非 Windows 平台的 Setsockopt 参数类型
#else
#define SetsockOptType const char *  // Windows 平台的 Setsockopt 参数类型
#endif

class Packet;

/**
 * @brief Network 类，用于管理网络连接和数据传输。
 * 
 * 该类继承自 Entity，并实现了 INetwork 接口，提供了基础的网络功能，包括创建连接、发送数据包等。
 */
class Network : public Entity<Network>, public INetwork
{
public:
    /**
     * @brief 将当前对象返回对象池，释放资源。
     */
    void BackToPool() override;

    /**
     * @brief 发送数据包，通过网络连接发送 Packet 对象。
     * 
     * @param pPacket 需要发送的 Packet 对象引用。
     */
    void SendPacket(Packet*& pPacket) override;

    /**
     * @brief 获取当前网络连接的类型。
     * 
     * @return NetworkType 网络连接类型，如 TcpListen。
     */
    NetworkType GetNetworkType() const { return _networkType; }

protected:
    /**
     * @brief 设置套接字的选项，例如禁用 Nagle 算法，设置超时等。
     * 
     * @param socket 套接字描述符。
     */
    void SetSocketOpt(SOCKET socket);

    /**
     * @brief 创建一个新的套接字，并根据不同平台进行适配。
     * 
     * @return SOCKET 创建的套接字描述符。
     */
    SOCKET CreateSocket();

    /**
     * @brief 检查套接字的有效性。
     * 
     * @param socket 需要检查的套接字。
     * @return bool 如果套接字有效返回 true，否则返回 false。
     */
    bool CheckSocket(SOCKET socket);

    /**
     * @brief 创建并管理一个连接对象。
     * 
     * @param socket 套接字描述符。
     * @param key 对象键，用于标识该连接。
     * @param iState 连接状态，如已连接、连接中等。
     * @return bool 创建成功返回 true，否则返回 false。
     */
    bool CreateConnectObj(SOCKET socket, ObjectKey key, ConnectStateType iState);

    // packet 操作

    /**
     * @brief 处理断开连接的数据包。
     * 
     * @param pPacket 包含断开连接信息的 Packet 对象。
     */
    void HandleDisconnect(Packet* pPacket);

#ifdef EPOLL
    /**
     * @brief 初始化 Epoll，适用于 Linux 平台的多路复用 I/O。
     */
    void InitEpoll();

    /**
     * @brief Epoll 的事件循环，用于处理网络 I/O 事件。
     */
    void Epoll();

    /**
     * @brief 添加 Epoll 事件。
     * 
     * @param epollfd Epoll 文件描述符。
     * @param fd 需要监听的文件描述符。
     * @param flag 事件标志，如可读、可写等。
     */
    void AddEvent(int epollfd, int fd, int flag);

    /**
     * @brief 修改 Epoll 事件。
     * 
     * @param epollfd Epoll 文件描述符。
     * @param fd 需要监听的文件描述符。
     * @param flag 事件标志。
     */
    void ModifyEvent(int epollfd, int fd, int flag);

    /**
     * @brief 删除 Epoll 事件。
     * 
     * @param epollfd Epoll 文件描述符。
     * @param fd 需要移除的文件描述符。
     */
    void DeleteEvent(int epollfd, int fd);

    /**
     * @brief Epoll 事件回调函数，需要子类实现。
     * 
     * @param fd 文件描述符。
     * @param index 事件索引。
     */
    virtual void OnEpoll(SOCKET fd, int index) { };
#else    
    /**
     * @brief 使用 Select 模型来处理网络 I/O 事件，适用于非 Epoll 环境。
     */
    void Select();
#endif

    /**
     * @brief 网络更新方法，定期调用以处理网络事件和数据。
     */
    void OnNetworkUpdate();

protected:
    // 当前网络连接的对象集合，使用套接字作为键
    std::map<SOCKET, ConnectObj*> _connects;

#ifdef EPOLL
#define MAX_CLIENT  5120  // 最大客户端连接数
#define MAX_EVENT   5120  // 最大事件数
    struct epoll_event _events[MAX_EVENT]; // Epoll 事件数组
    int _epfd{ -1 }; // Epoll 文件描述符
#else
    SOCKET _fdMax{ INVALID_SOCKET }; // 最大套接字文件描述符
    fd_set readfds, writefds, exceptfds; // 套接字集合，用于 Select
#endif

    // 网络包的缓存队列，存储待发送的消息
    std::mutex _sendMsgMutex;
    CacheSwap<Packet> _sendMsgList;

    // 当前网络的类型，默认为 TcpListen
    NetworkType _networkType{ NetworkType::TcpListen };
};
