#include "network.h"  // 包含 Network 类的头文件
#include "connect_obj.h"  // 包含 ConnectObj 类的头文件
#include <iostream>  // 包含标准输入输出流库

// Dispose 方法，释放所有连接对象并关闭主 socket
void Network::Dispose() {
    auto iter = _connects.begin();  // 获取连接对象的迭代器
    while (iter != _connects.end()) {
        iter->second->Dispose();  // 释放连接对象资源
        delete iter->second;  // 删除连接对象
        ++iter;
    }
    _connects.clear();  // 清空连接对象集合
    _sock_close(_masterSocket);  // 关闭主 socket
    _masterSocket = -1;  // 重置主 socket
}

#ifndef WIN32
#define SetsockOptType void *
#else 
#define SetsockOptType const char *
#endif

// 设置 socket 选项
void Network::SetSocketOpt(SOCKET socket) {
    bool isReuseaddr = true;

    /* 功能
        设置地址复用选项：允许多个套接字绑定到同一个地址和端口。
        适用场景：当服务器需要在短时间内重新启动时，能够立即绑定到先前使用的地址和端口，而无需等待操作系统释放端口。
        socket：要设置选项的套接字描述符。
        SOL_SOCKET：指示套接字级别的选项。
        SO_REUSEADDR：允许端口在 TIME_WAIT 状态下被重复使用。
        (SetsockOptType)&isReuseaddr：指向包含选项值的变量的指针。在这里，isReuseaddr 为 true，表示启用该选项。
        sizeof(isReuseaddr)：选项值的大小。
    */
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (SetsockOptType)&isReuseaddr, sizeof(isReuseaddr));  // 设置地址复用

    int netTimeout = 3000;  // 超时时间为 3000 毫秒
    /* 功能
        设置发送超时：控制发送操作的超时时间。
        适用场景：当发送操作未能在指定时间内完成时，发送操作将超时并返回错误。
        SO_SNDTIMEO：设置发送数据的超时时间。
        netTimeout：超时时间（单位：毫秒）。在这里，设置为 3000 毫秒（3 秒）。
    */
    setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (SetsockOptType)&netTimeout, sizeof(netTimeout));  // 发送超时
    /* 功能
        设置接收超时：控制接收操作的超时时间。
        适用场景：当接收操作未能在指定时间内完成时，接收操作将超时并返回错误。
        SO_RCVTIMEO：设置接收数据的超时时间。
        netTimeout：超时时间（单位：毫秒）。在这里，设置为 3000 毫秒（3 秒）。
    */
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (SetsockOptType)&netTimeout, sizeof(netTimeout));  // 接收超时
#ifndef WIN32

    int keepAlive = 1;  // 启用保活机制
    socklen_t optlen = sizeof(keepAlive);
    int keepIdle = 60 * 2;  // 空闲 2 分钟后开始保活探测
    int keepInterval = 10;  // 每隔 10 秒进行一次保活探测
    int keepCount = 5;  // 连续探测 5 次失败则认为连接断开

    /* 功能
        启用保活选项：允许系统发送保活探测包。
        启用 TCP 保活：在连接空闲时，定期发送保活探测包以确认连接仍然有效。
        适用场景：确保长时间未活动的连接在网络不可达时能被及时发现并关闭。
        SO_KEEPALIVE：启用保活机制。
        keepIdle：空闲 120 秒后开始发送保活探测包。
        keepInterval：每隔 10 秒发送一次保活探测包。
        keepCount：如果连续 5 次保活探测包未获得响应，则认为连接已断开。
    */
    setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (SetsockOptType)&keepAlive, optlen);  // 设置保活选项
    if (getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, &optlen) < 0) {
        std::cout << "getsockopt SO_KEEPALIVE failed." << std::endl;
    }
    setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, optlen);  // 设置保活选项
    if (getsockopt(socket, SOL_TCP, TCP_KEEPIDLE, &keepIdle, &optlen) < 0) {    // 检查保活选项是否设置成功
        std::cout << "getsockopt TCP_KEEPIDLE failed." << std::endl;
    }
    /* 功能
        设置保活探测间隔：控制发送保活探测包的间隔时间。
    */
    setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, optlen);  // 设置保活探测间隔
    /* 功能
        设置保活探测次数：控制在认定连接断开之前发送的探测包的数量。
    */
    setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, optlen);  // 设置保活探测次数

#endif
    _sock_nonblock(socket);  // 设置 socket 为非阻塞模式
}

// 创建并返回一个新的 socket
SOCKET Network::CreateSocket() {
    _sock_init();  // 初始化 socket 库
    SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // 创建 TCP socket
    if (socket == INVALID_SOCKET) {
        std::cout << "::socket failed. err: " << _sock_err() << std::endl;
        return socket;  // 如果创建失败，返回错误的 socket
    }
    SetSocketOpt(socket);  // 设置 socket 选项
    return socket;  // 返回创建的 socket
}

// 选择可读、可写和异常状态的 socket
bool Network::Select() {
    FD_ZERO(&readfds);  // 清空可读文件描述符集合
    FD_ZERO(&writefds);  // 清空可写文件描述符集合
    FD_ZERO(&exceptfds);  // 清空异常文件描述符集合

    FD_SET(_masterSocket, &readfds);  // 将主 socket 添加到可读集合
    FD_SET(_masterSocket, &writefds);  // 将主 socket 添加到可写集合
    FD_SET(_masterSocket, &exceptfds);  // 将主 socket 添加到异常集合

    SOCKET fdmax = _masterSocket;  // 设置最大文件描述符为主 socket
    for (auto iter = _connects.begin(); iter != _connects.end(); ++iter) {
        if (iter->first > fdmax)
            fdmax = iter->first;  // 更新最大文件描述符
        FD_SET(iter->first, &readfds);  // 将连接 socket 添加到可读集合
        FD_SET(iter->first, &exceptfds);  // 将连接 socket 添加到异常集合

        if (iter->second->HasSendData()) {
            FD_SET(iter->first, &writefds);  // 如果有发送数据，将连接 socket 添加到可写集合
        } else {
            if (_masterSocket == iter->first)
                FD_CLR(_masterSocket, &writefds);  // 如果没有发送数据，从可写集合中移除主 socket
        }
    }

    struct timeval timeout;
    timeout.tv_sec = 0;  // 设置超时时间为 0 秒
    timeout.tv_usec = 50 * 1000;  // 设置超时时间为 50 毫秒
    int nfds = ::select(fdmax + 1, &readfds, &writefds, &exceptfds, &timeout);  // 执行 select 调用
    if (nfds <= 0)
        return true;  // 如果没有文件描述符准备好，返回 true

    auto iter = _connects.begin();
    while (iter != _connects.end()) {
        if (FD_ISSET(iter->first, &exceptfds)) {
            std::cout << "socket except!! socket: " << iter->first << std::endl;
            iter->second->Dispose();  // 处理异常 socket
            delete iter->second;  // 删除异常 socket
            iter = _connects.erase(iter);  // 从集合中移除
            continue;
        }

        if (FD_ISSET(iter->first, &readfds)) {
            if (!iter->second->Recv()) {
                iter->second->Dispose();  // 处理接收失败
                delete iter->second;  // 删除接收失败的 socket
                iter = _connects.erase(iter);  // 从集合中移除
                continue;
            }
        }

        if (FD_ISSET(iter->first, &writefds)) {
            if (!iter->second->Send()) {
                iter->second->Dispose();  // 处理发送失败
                delete iter->second;  // 删除发送失败的 socket
                iter = _connects.erase(iter);  // 从集合中移除
                continue;
            }
        }
        ++iter;  // 移动到下一个连接
    }
    return true;  // 返回 true 表示选择操作成功
}
