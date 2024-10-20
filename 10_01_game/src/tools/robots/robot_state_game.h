#pragma once

#include "robot_state.h"

// 游戏连接状态类
class RobotStateGameConnecting : public RobotState
{
public:
    // 定义动态状态创建，状态类型为 Game_Connecting
    DynamicStateCreate(RobotStateGameConnecting, RobotStateType::Game_Connecting);

    // 当进入游戏连接状态时调用
    void OnEnterState() override;

    // 更新游戏连接状态的方法
    RobotStateType OnUpdate() override;
};

// 游戏已连接状态类
class RobotStateGameConnected : public RobotState
{
public:
    // 定义动态状态创建，状态类型为 Game_Connected
    DynamicStateCreate(RobotStateGameConnected, RobotStateType::Game_Connected);

    // 当进入游戏已连接状态时调用
    void OnEnterState() override;
};

// 游戏已登录状态类
class RobotStateGameLogined : public RobotState
{
public:
    // 定义动态状态创建，状态类型为 Game_Logined
    DynamicStateCreate(RobotStateGameLogined, RobotStateType::Game_Logined);
};
