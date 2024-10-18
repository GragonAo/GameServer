#pragma once

#include "robot_state.h"
#include "libserver/common.h"

// 处理机器人连接登录服务器时的状态类
class RobotStateLoginConnecting : public RobotState {
public:
    // 使用宏生成状态的创建函数
    // 传入当前状态类型为 Login_Connecting
    DynamicStateCreate(RobotStateLoginConnecting, RobotStateType::Login_Connecting);

    // 进入此状态时的处理逻辑，通常用于建立网络连接
    void OnEnterState() override;
    
    // 状态更新函数，用于检查网络连接的状态并决定是否切换到下一个状态
    RobotStateType OnUpdate() override;
};

// 处理机器人已连接到登录服务器时的状态类
class RobotStateLoginConnected : public RobotState {
public:
    // 使用宏生成状态的创建函数
    // 传入当前状态类型为 Login_Connected
    DynamicStateCreate(RobotStateLoginConnected, RobotStateType::Login_Connected);

    // 进入此状态时的处理逻辑，通常用于发送账户校验请求
    void OnEnterState() override;
    
    // 状态更新函数，判断账户校验结果，决定是否切换到下一个状态
    RobotStateType OnUpdate() override;
};

// 处理机器人登录成功后的状态类
class RobotStateLoginLogined : public RobotState {
public:
    // 使用宏生成状态的创建函数
    // 传入当前状态类型为 Login_Logined
    DynamicStateCreate(RobotStateLoginLogined, RobotStateType::Login_Logined);

    // 状态更新函数，处理机器人登录成功后的行为
    RobotStateType OnUpdate() override;
};

// 处理机器人选择角色的状态类
class RobotStateLoginSelectPlayer : public RobotState {
public:
    // 使用宏生成状态的创建函数
    // 传入当前状态类型为 Login_SelectPlayer
    DynamicStateCreate(RobotStateLoginSelectPlayer, RobotStateType::Login_SelectPlayer);
};
