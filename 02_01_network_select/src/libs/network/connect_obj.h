#pragma once  // 防止头文件被多次包含

#include "disposable.h"  // 包含 IDisposable 类的定义

class Network;
class RecvNetworkBuffer;
class SendNetworkBuffer;
class Packet;

// ConnectObj 类继承自 IDisposable
class ConnectObj : public IDisposable {
    public:
        // 构造函数，初始化 Network 指针和 socket
        ConnectObj(Network* pNetWork, int socket);
        // 析构函数，重写自基类
        ~ConnectObj() override;

        // 实现 IDisposable 接口的 Dispose 方法
        void Dispose() override;

        // 获取 socket 值
        int GetSocket() const { return _socket; }

        // 检查是否有接收数据
        bool HasRecData() const;

        // 获取接收到的包
        Packet* GetRecvPacket() const;

        // 执行接收操作
        bool Recv() const;

        // 检查是否有发送数据
        bool HasSendData() const;

        // 发送数据包
        void SendPacket(Packet* pPacket) const;

        // 执行发送操作
        bool Send() const;

    protected:
        Network* _pNetWork{nullptr};  // 网络指针
        const int _socket;  // socket 描述符
        RecvNetworkBuffer* _recvBuffer{nullptr};  // 接收缓冲区
        SendNetworkBuffer* _sendBuffer{nullptr};  // 发送缓冲区
};
