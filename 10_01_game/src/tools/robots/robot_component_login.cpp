#include "robot_component_login.h"

// 机器人组件登录的具体实现

// Awake 方法用于初始化机器人登录的 IP 地址和端口
// 参数：ip - 登录服务器的 IP 地址，port - 登录服务器的端口号
void RobotComponentLogin::Awake(std::string ip, int port)
{
    _loginIp = ip;   // 设置登录 IP
    _loginPort = port; // 设置登录端口
}

// BackToPool 方法用于将对象归还到对象池时执行的清理操作
void RobotComponentLogin::BackToPool()
{

}
