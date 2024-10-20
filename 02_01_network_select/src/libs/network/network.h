#pragma once

#include "disposable.h"

#include <map>

#ifndef WIN32 // 如果不是在 Windows 平台上
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#define SOCKET int
#define INVALID_SOCKET -1

#define _sock_init() // UNIX 平台下不需要初始化
/*
    { int flags = fcntl(sockfd, F_GETFL, 0); }：

fcntl：文件控制函数，用于操作文件描述符。
sockfd：要设置为非阻塞模式的套接字文件描述符。
F_GETFL：获取文件状态标志。
flags：保存套接字的当前文件状态标志。
详细步骤：
调用 fcntl(sockfd, F_GETFL, 0) 获取当前文件状态标志，并将其保存到变量 flags 中。
fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);：
F_SETFL：设置文件状态标志。
O_NONBLOCK：非阻塞标志。
详细步骤：
调用 fcntl(sockfd, F_SETFL, flags | O_NONBLOCK)，将当前文件状态标志 flags 与非阻塞标志 O_NONBLOCK 进行按位或操作，设置套接字为非阻塞模式。
*/
#define _sock_nonblock(sockfd) { int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); } // 设置非阻塞模式
#define _sock_exit() // UNIX 平台下不需要清理
#define _sock_err() errno // 获取错误代码
#define _sock_close(sockfd) ::close(sockfd) // 关闭套接字
#define _sock_is_blocked() (errno == EAGAIN || errno == 0) // 检查是否为阻塞错误

#else // 如果是在 Windows 平台上

#define FD_SETSIZE 1024

#include <Ws2tcpip.h>
#include <windows.h>

#define _sock_init() { WSADATA wsaData; WSAStartup(MAKEWORD(2, 2), &wsaData); } // 初始化套接字库
#define _sock_nonblock(sockfd) { unsigned long param = 1; ioctlsocket(sockfd, FIONBIO, &param); } // 设置非阻塞模式
#define _sock_exit() { WSACleanup(); } // 清理套接字库
#define _sock_err() WSAGetLastError() // 获取错误代码
#define _sock_close(sockfd) ::closesocket(sockfd) // 关闭套接字
#define _sock_is_blocked() (WSAGetLastError() == WSAEWOULDBLOCK) // 检查是否为阻塞错误

#endif

class ConnectObj;

class Network : public IDisposable {
public:
    void Dispose() override; // 释放资源的虚函数
    bool Select(); // 进行选择操作

    SOCKET GetSocket() const { return _masterSocket; } // 获取主套接字

protected:
    static void SetSocketOpt(SOCKET socket); // 设置套接字选项
    SOCKET CreateSocket(); // 创建一个新的套接字

protected:
    SOCKET _masterSocket{ INVALID_SOCKET }; // 主套接字
    std::map<SOCKET, ConnectObj*> _connects; // 套接字到连接对象的映射
    fd_set readfds, writefds, exceptfds; // 文件描述符集合，用于 select 操作
};