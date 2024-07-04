#include <iostream>  // 包含标准输入输出流库
#include "connect_obj.h"  // 包含 ConnectObj 类的头文件
#include "network.h"  // 包含 Network 类的头文件
#include "network_buffer.h"  // 包含网络缓冲区类的头文件
#include "packet.h"  // 包含 Packet 类的头文件

// ConnectObj 构造函数，初始化网络指针和 socket，并分配接收和发送缓冲区
ConnectObj::ConnectObj(Network *pNetWork, int socket) 
    : _pNetWork(pNetWork), _socket(socket) {
    _recvBuffer = new RecvNetworkBuffer(DEFAULT_RECV_BUFFER_SIZE);  // 分配接收缓冲区
    _sendBuffer = new SendNetworkBuffer(DEFAULT_SEND_BUFFER_SIZE);  // 分配发送缓冲区
}

// ConnectObj 析构函数，释放接收和发送缓冲区
ConnectObj::~ConnectObj() {
    delete _recvBuffer;
    delete _sendBuffer;
}

// Dispose 方法，关闭 socket 并释放缓冲区资源
void ConnectObj::Dispose() {
    std::cout << "close socket: " << _socket << std::endl;
    _sock_close(_socket);  // 关闭 socket

    _recvBuffer->Dispose();  // 释放接收缓冲区资源
    _sendBuffer->Dispose();  // 释放发送缓冲区资源
}

// 检查是否有接收数据
bool ConnectObj::HasRecData() const {
    return _recvBuffer->HasData();
}

// 获取接收到的包
Packet* ConnectObj::GetRecvPacket() const {
    return _recvBuffer->GetPacket();
}

// 执行接收操作
bool ConnectObj::Recv() const {
    char *pBuffer = nullptr;
    while (true) {
        // 检查接收缓冲区是否有足够的空间存放新数据包
        if (_recvBuffer->GetEmptySize() < sizeof(Packet)) {
            _recvBuffer->ReAllocBuffer();
        }
        // 获取缓冲区的空闲空间大小
        const int emptySize = _recvBuffer->GetBuffer(pBuffer);
        // 从 socket 接收数据到缓冲区
        const int dataSize = ::recv(_socket, pBuffer, emptySize,0);
        if (dataSize > 0) {
            _recvBuffer->FillDate(dataSize);  // 修改缓冲区的下标位置
        } else if (dataSize == 0) {
            return false;  // 连接已关闭
        } else {
            const auto socketError = _sock_err();
#ifndef WIN32
            if (socketError == EINTR || socketError == EWOULDBLOCK || socketError == EAGAIN)
                return true;  // 非阻塞模式下继续接收
#else
            if (socketError == WSAEINTR || socketError == WSAEWOULDBLOCK)
                return true;  // 非阻塞模式下继续接收
#endif
            return false;  // 其他错误
        }
    }
}

// 检查是否有发送数据
bool ConnectObj::HasSendData() const {
    return _sendBuffer->HasData();
}

// 发送数据包
void ConnectObj::SendPacket(Packet *pPacket) const {
    _sendBuffer->AddPacket(pPacket);  // 将数据包添加到发送缓冲区
}

// 执行发送操作
bool ConnectObj::Send() const {
    while (true) {
        char *pBuffer = nullptr;
        // 获取需要发送的数据大小
        const int needSendSize = _sendBuffer->GetBuffer(pBuffer);

        if (needSendSize <= 0)
            return true;  // 没有数据需要发送
        // 发送数据到 socket
        const int size = ::send(_socket, pBuffer, needSendSize,0);
        if (size > 0) {
            _sendBuffer->RemoveData(size);  // 从发送缓冲区中移除已发送的数据
            if (size < needSendSize)
                return true;  // 还有剩余数据需要发送
        } else if (size == -1) {
            const auto socketError = _sock_err();
            std::cout << "needSendSize: " << needSendSize << std::endl;
#ifndef WIN32
            if (socketError == EINTR || socketError == EWOULDBLOCK || socketError == EAGAIN)
                return true;  // 非阻塞模式下继续发送
#else
            if (socketError == WSAEINTR || socketError == WSAEWOULDBLOCK)
                return true;  // 非阻塞模式下继续发送
#endif
            return false;  // 其他错误
        }
    }
}
