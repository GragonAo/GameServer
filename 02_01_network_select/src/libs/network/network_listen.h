#pragma once
#include "network.h"

class NetworkListen : public Network {
public:
    // 监听指定IP和端口
    bool Listen(std::string ip, int port);

    // 更新连接状态
    bool Update();

protected:
    // 接受客户端连接
    virtual int Accept();
};
