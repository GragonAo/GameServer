#include <iostream>
#include "network_listen.h"
#include "connect_obj.h"
#include <cstring>
// 监听指定IP和端口
bool NetworkListen::Listen(std::string ip, int port) {
    _masterSocket = CreateSocket(); // 创建套接字
    if(_masterSocket == INVALID_SOCKET) // 如果创建失败
        return false; // 返回失败

    sockaddr_in addr; // 创建地址结构体
    memset(&addr, 0, sizeof(sockaddr_in)); // 初始化为0
    addr.sin_family = AF_INET; // 设置地址族为IPv4
    addr.sin_port = htons(port); // 设置端口号
    ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr); // 将IP地址转换为网络字节序并设置到地址结构体中
    
    // 绑定套接字到指定地址和端口
    if(::bind(_masterSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cout << "::bind failed. err : " << _sock_err() << std::endl; // 输出绑定失败消息
        return false; // 返回失败
    }

    int backlog = 10; // 设置连接队列的最大长度
#ifndef WIN32
    char* ptr;
    if((ptr = getenv("LISTENQ")) != nullptr) // 从环境变量中获取连接队列长度
        backlog = atoi(ptr); // 转换为整数
#endif
    // 开始监听连接
    if(::listen(_masterSocket, backlog) < 0) {
        std::cout << " ::listen failed. " << _sock_err() << std::endl; // 输出监听失败消息
        return false; // 返回失败
    }
    return true; // 返回成功
}

// 接受客户端连接
int NetworkListen::Accept() {
    struct sockaddr socketClient; // 客户端套接字地址信息
    socklen_t socketLength = sizeof(socketClient); // 地址信息长度

    int rs = 0; // 接受连接的数量
    while (true) {
        SOCKET socket = ::accept(_masterSocket, &socketClient, &socketLength); // 接受客户端连接
        if(socket == INVALID_SOCKET) // 如果连接失败
            return rs; // 返回已接受的连接数量
        SetSocketOpt(socket); // 设置套接字选项
        ConnectObj* pConnectObj = new ConnectObj(this, socket); // 创建连接对象
        _connects.insert(std::make_pair(socket, pConnectObj)); // 将连接对象插入映射
        ++rs; // 增加已接受连接数量
    }
}

// 更新连接状态
bool NetworkListen::Update() {
    bool br = Select(); // 调用Select函数更新连接状态
    if(FD_ISSET(_masterSocket, &readfds)) { // 如果主套接字可读
        Accept(); // 接受客户端连接
    }
    return br; // 返回连接状态
}
