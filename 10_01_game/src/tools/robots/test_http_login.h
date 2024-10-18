#pragma once
#include "libserver/entity.h"
#include "libserver/system.h"
#include "libserver/network.h"

// TestHttpLogin 类继承自 NetworkIdentify、Entity 和 IAwakeSystem
// 主要用于处理 HTTP 登录请求，负责处理连接、断开连接和响应等网络事件
class TestHttpLogin : public NetworkIdentify, 
                      public Entity<TestHttpLogin>, 
                      public IAwakeSystem<std::string, std::string>
{
public:
    // 实现自 IAwakeSystem，初始化登录信息
    // account 表示用户账户，password 表示登录密码
    void Awake(std::string account, std::string password) override;

    // 返回对象到内存池时的清理操作
    void BackToPool() override;

protected:
    // 处理网络断开事件
    void HandleNetworkDisconnect(TestHttpLogin* pObj, Packet* pPacket);
    
    // 处理网络连接事件
    void HandleNetworkConnected(TestHttpLogin* pObj, Packet* pPacket);

    // 处理外部 HTTP 响应
    void HandleHttpOuterResponse(TestHttpLogin* pObj, Packet* pPacket);

private:
    // 私有成员变量，存储账户、密码、IP、端口和 HTTP 方法
    std::string _account{ "" };    // 用户账号
    std::string _password{ "" };   // 登录密码
    std::string _ip{ "" };         // IP 地址
    int _port{ 0 };                // 端口号
    std::string _mothed{ "" };     // HTTP 请求方法
    std::string _path{""};  //HTTP 路径
};
