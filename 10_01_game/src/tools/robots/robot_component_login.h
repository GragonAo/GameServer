#pragma once
#include "libserver/component.h"
#include "libserver/system.h"

// RobotComponentLogin 类负责管理机器人登录信息（IP 和端口）
class RobotComponentLogin : public Component<RobotComponentLogin>, 
                            public IAwakeFromPoolSystem<std::string, int>
{
public:
    // 当对象从对象池中取出并初始化时调用
    // 参数：ip - 登录服务器的 IP 地址，port - 登录服务器的端口号
    void Awake(std::string ip, int port) override;

    // 当对象返回对象池时调用，用于清理资源
    void BackToPool() override;

    // 获取登录服务器的 IP 地址
    std::string GetLoginIp() const { return _loginIp; }

    // 获取登录服务器的端口号
    int GetLoginPort() const { return _loginPort; }

private:
    // 登录服务器的 IP 地址
    std::string _loginIp{ "" };

    // 登录服务器的端口号
    int _loginPort{ 0 };
};
