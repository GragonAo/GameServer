#include "robot_component_gametoken.h"

// 初始化游戏令牌、IP 和端口
// 参数：
// token - 登录游戏的令牌
// ip - 游戏服务器的 IP 地址
// port - 游戏服务器的端口号
void RobotComponentGameToken::Awake(std::string token, std::string ip, int port)
{
    // 将参数赋值给私有成员变量，存储游戏的令牌、IP 和端口信息
    _token = token;
    _gameIp = ip;
    _gamePort = port;
}

// 回收对象时的处理逻辑
// 注意：此处可以添加逻辑来重置或清理对象的数据，以便对象重新使用时处于初始状态
void RobotComponentGameToken::BackToPool()
{
    // _token = ""; _gameIp = ""; _gamePort = 0;
}
