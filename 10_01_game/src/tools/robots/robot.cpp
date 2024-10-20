#include "robot.h"

// 包含机器人状态的头文件
#include "robot_state_login.h"       // 登录相关状态
#include "robot_state_http.h"        // HTTP 连接相关状态
#include "robot_state_game.h"        // 游戏相关状态
#include "global_robots.h"           // 全局机器人管理

// 包含服务器相关的库文件
#include "libserver/common.h"        // 通用功能
#include "libserver/robot_state_type.h"  // 机器人状态类型定义
#include "libserver/entity_system.h" // 实体系统
#include "libserver/log4_help.h"     // 日志帮助函数
#include "libserver/message_system_help.h" // 消息系统帮助函数
#include "libserver/update_component.h" // 更新组件

#include <json/reader.h>             // JSON 解析库

#include "libserver/message_system.h" // 消息系统

// 从对象池唤醒机器人
void Robot::Awake(std::string account)
{
    // 调用 Player 类的 Awake 方法，传入账号
    Player::Awake(nullptr, account);

    // 添加更新组件，绑定更新函数
    AddComponent<UpdateComponent>(BindFunP0(this, &Robot::Update));

    // 初始化状态模板管理器，设置初始状态为 Http_Connecting
    InitStateTemplateMgr(RobotStateType::Http_Connecting);
}

// 返回对象池时的处理逻辑
void Robot::BackToPool()
{
    _account = ""; // 清空账号信息
    Player::BackToPool(); // 调用 Player 类的 BackToPool 方法
}

// 更新方法，每帧调用
void Robot::Update()
{
    // 更新当前状态
    UpdateState();
}

// 处理网络断开连接的逻辑
void Robot::NetworkDisconnect()
{
    // 如果套接字无效，则返回
    if (_socketKey.Socket == INVALID_SOCKET)
        return;

    // 分发网络断开请求的消息
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect, this);
    
    // 清理套接字键
    _socketKey.Clean();
}

// 设置机器人套接字键
void Robot::SetSocketKey(SocketKey key)
{
    _socketKey = key; // 赋值套接字键
}

// 注册机器人状态
void Robot::RegisterState()
{
    // 注册 HTTP 连接相关的状态
    RegisterStateClass(RobotStateType::Http_Connecting, DynamicStateBind(RobotStateHttpConnecting));
    RegisterStateClass(RobotStateType::Http_Connected, DynamicStateBind(RobotStateHttpConnected));

    // 注册登录相关的状态
    RegisterStateClass(RobotStateType::Login_Connecting, DynamicStateBind(RobotStateLoginConnecting));
    RegisterStateClass(RobotStateType::Login_Connected, DynamicStateBind(RobotStateLoginConnected));
    RegisterStateClass(RobotStateType::Login_Logined, DynamicStateBind(RobotStateLoginLogined));
    RegisterStateClass(RobotStateType::Login_SelectPlayer, DynamicStateBind(RobotStateLoginSelectPlayer));

    // 注册游戏相关的状态
    RegisterStateClass(RobotStateType::Game_Connecting, DynamicStateBind(RobotStateGameConnecting));
    RegisterStateClass(RobotStateType::Game_Connected, DynamicStateBind(RobotStateGameConnected));
}
