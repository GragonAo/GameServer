#pragma once
#include "libserver/component.h"
#include "libserver/system.h"

// 机器人游戏令牌组件类，用于存储机器人登录游戏服务器所需的令牌、IP 和端口信息
class RobotComponentGameToken : public Component<RobotComponentGameToken>, public IAwakeFromPoolSystem<std::string, std::string, int>
{
public:
    // 初始化函数，用于设置游戏令牌、IP 和端口
    // 参数：
    // token - 登录游戏的令牌
    // ip - 游戏服务器的 IP 地址
    // port - 游戏服务器的端口号
    void Awake(std::string token, std::string ip, int port) override;

    // 重置对象池的回调函数，当对象返回对象池时调用，清空存储的令牌、IP 和端口信息
    void BackToPool() override;

    // 获取游戏登录令牌
    std::string GetToken() const { return _token; }

    // 获取游戏服务器 IP 地址
    std::string GetGameIp() const { return _gameIp; }

    // 获取游戏服务器端口号
    int GetGamePort() const { return _gamePort; }

private:
    // 保存游戏登录令牌
    std::string _token{ "" };
    
    // 保存游戏服务器的 IP 地址
    std::string _gameIp{ "" };
    
    // 保存游戏服务器的端口号
    int _gamePort{ 0 };
};
