#include <iostream>
#include "connect_obj.h"
#include "network_connector.h"
#include "packet.h"
#include <cstring>
// 检查网络连接是否存在
bool NetworkConnector::IsConnected() const {
    return _connects.size() > 0; // 如果连接数大于0，返回true，否则返回false
}

// 获取连接对象
ConnectObj* NetworkConnector::GetConnectObj() {
    auto iter = _connects.find(_masterSocket); // 在连接映射中查找主套接字对应的连接对象
    if(iter == _connects.end()) { // 如果未找到连接对象
        std::cout << "Error. NetworkConnectorpConnect == nullptr!!!" << std::endl; // 输出错误消息
        return nullptr; // 返回空指针
    }
    return iter->second; // 返回找到的连接对象
}

// 连接到指定IP和端口
bool NetworkConnector::Connect(std::string ip, int port) {
    _ip = ip; // 设置IP地址
    _port = port; // 设置端口号

    _masterSocket = CreateSocket(); // 创建套接字
    if(_masterSocket == INVALID_SOCKET) // 如果创建失败
        return false; // 返回失败

    sockaddr_in addr; // 创建地址结构体
    memset(&addr, 0, sizeof(sockaddr_in)); // 初始化为0
    addr.sin_family = AF_INET; // 设置地址族为IPv4
    addr.sin_port = htons(_port); // 设置端口号
    ::inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr.s_addr); // 将IP地址转换为网络字节序并设置到地址结构体中
    int rs = ::connect(_masterSocket, (struct sockaddr *)&addr, sizeof(sockaddr)); // 连接到指定地址
    if(rs == 0) { // 如果连接成功
        ConnectObj* pConnectObj = new ConnectObj(this, _masterSocket); // 创建连接对象
        _connects.insert(std::make_pair(_masterSocket, pConnectObj)); // 将连接对象插入映射
    }
    return true; // 返回成功
}

// 更新连接状态
bool NetworkConnector::Update() {
    const bool br = Select(); // 调用Select函数更新连接状态
    if(!IsConnected()) { // 如果未连接
        if(FD_ISSET(_masterSocket, &exceptfds)) { // 如果主套接字出现异常
            std::cout<< "connect except. socket : " << _masterSocket << " re connect." << std::endl; // 输出重新连接的消息
            Dispose(); // 清理资源
            Connect(_ip, _port); // 重新连接
            return br; // 返回连接状态
        }
        if(FD_ISSET(_masterSocket, &readfds) || FD_ISSET(_masterSocket, &writefds)) { // 如果主套接字可读或可写
            int optval = -1; // 用于存储选项值
            socklen_t optlen = sizeof(_masterSocket); // 选项长度
            int re = getsockopt(_masterSocket, SOL_SOCKET, SO_ERROR, (char *)(&optval), &optlen); // 获取套接字选项
            if(re == 0 && optval == 0) { // 如果选项获取成功且值为0
                ConnectObj* pConnectObj = new ConnectObj(this, _masterSocket); // 创建连接对象
                _connects.insert(std::make_pair(_masterSocket, pConnectObj)); // 将连接对象插入映射
            } else { // 如果选项获取失败或值不为0
                std::cout << "connect failed. socket:" << _masterSocket << std::endl; // 输出连接失败消息
                Dispose(); // 清理资源
                Connect(_ip, _port); // 重新连接
            }
        }
    }
    return br; // 返回连接状态
}

// 检查是否有接收到数据
bool NetworkConnector::HasRecvData() {
    int size = _connects.size(); // 获取连接数
    if(size <= 0) // 如果连接数小于等于0
        return false; // 返回false
    if(size != 1) { // 如果连接数不为1
        std::cout << "Error. NetworkConnect has two connect!!!" << std::endl; // 输出错误消息
        return false; // 返回false
    }
    ConnectObj* pConnect = GetConnectObj(); // 获取连接对象
    if(pConnect == nullptr) // 如果连接对象为空
        return false; // 返回false
    return pConnect->HasRecData(); // 返回连接对象是否有接收到数据
}

// 获取接收到的数据包
Packet* NetworkConnector::GetRecvPacket() {
    ConnectObj* pConnect = GetConnectObj(); // 获取连接对象
    if(pConnect == nullptr) // 如果连接对象为空
        return nullptr; // 返回空指针
    return pConnect->GetRecvPacket(); // 返回连接对象的接收数据包
}

// 发送数据包
void NetworkConnector::SendPacket(Packet* pPacket) {
    ConnectObj* pConnect = GetConnectObj(); // 获取连接对象
    if(pConnect == nullptr) // 如果连接对象为空
        return; // 返回
    pConnect->SendPacket(pPacket); // 发送数据包
}
