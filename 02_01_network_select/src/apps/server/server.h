#pragma once                         // 防止头文件重复包含
#include "network/network_listen.h"  // 引入 network_listen 头文件

// 定义 Server 类，继承自 NetworkListen 类
class Server : public NetworkListen {
    public:
        bool DataHandler();          // 声明一个公共成员函数 DataHandler，用于处理数据
    protected:
        void HandlerOne(ConnectObj* pConnectObj);  // 声明一个受保护成员函数 HandlerOne，用于处理单个连接对象
        int Accept() override;       // 重写基类的 Accept 函数，处理新的连接请求
    protected:
        int _acceptCount{0};         // 记录接受的连接数
        int _sendMsgCount{0};        // 记录发送的消息数
        int _recvMsgCount{0};        // 记录接收的消息数

        bool _isShow{true};          // 标志是否显示统计信息
};
