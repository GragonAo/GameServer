#pragma once
#include "network.h"

class ConnectObj;
class Packet;

//类用于管理网络连接。
class NetworkConnector : public Network {
public:

    //连接到指定 IP 和端口。
    bool Connect(std::string ip, int port);

    //更新网络连接状态。
    bool Update();

    //检查是否有接收数据。
    bool HasRecvData();

    //获取接收到的数据包。
    Packet* GetRecvPacket();

    //发送数据包。
    void SendPacket(Packet* pPacket);

    //检查连接是否已建立。
    bool IsConnected() const;

private:
    //获取连接对象。
    ConnectObj* GetConnectObj();

private:
    std::string _ip; // 目标 IP 地址
    int _port;       // 目标端口号
};
