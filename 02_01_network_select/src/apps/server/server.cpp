#include <iostream>                    // 引入标准输入输出流库
#include "server.h"                    // 引入 server 头文件
#include "network/packet.h"            // 引入 packet 头文件
#include "network/connect_obj.h"       // 引入 connect_obj 头文件

// 用于处理数据
bool Server::DataHandler() {
    // 遍历所有连接对象
    for (auto iter = _connects.begin(); iter != _connects.end(); ++iter) {
        ConnectObj* pConnectObj = iter->second;   // 获取当前连接对象
        HandlerOne(pConnectObj);                  // 处理单个连接对象的数据
    }
    // 如果 _isShow 标志为真，输出统计信息
    if (_isShow) {
        _isShow = false;  // 重置 _isShow 标志
        // 输出接受的连接数、接收消息数和发送消息数
        std::cout << "accept:" << _acceptCount << "\trecv count:" << _recvMsgCount << "\tsend count:" << _sendMsgCount << std::endl;
    }
    return true;  // 返回 true 表示处理成功
}

// 处理单个连接对象的数据
void Server::HandlerOne(ConnectObj* pConnectObj) {
    // 当连接对象有接收到的数据时，处理数据
    while (pConnectObj->HasRecData()) {
        Packet* pPacket = pConnectObj->GetRecvPacket();  // 获取接收到的包
        if (pPacket == nullptr) break;  // 如果没有包，跳出循环

        // 将包中的数据转换为字符串
        std::string msg(pPacket->GetBuffer(), pPacket->GetDataLength());
        // 输出接收到的消息大小和消息内容
        std::cout << "recv size:" << msg.length() << " msg:" << msg.c_str() << std::endl;
        pConnectObj->SendPacket(pPacket);  // 发送接收到的包
        ++_recvMsgCount;  // 接收消息计数加一
        ++_sendMsgCount;  // 发送消息计数加一
        _isShow = true;   // 设置 _isShow 标志，表示有统计信息需要输出
    }
}

// 处理新的连接请求
int Server::Accept() {
    int rs = NetworkListen::Accept();  // 调用基类的 Accept 函数，处理连接请求
    _acceptCount += rs;  // 累加接受的连接数
    _isShow = true;      // 设置 _isShow 标志，表示有统计信息需要输出
    return rs;  // 返回接受的连接数
}
