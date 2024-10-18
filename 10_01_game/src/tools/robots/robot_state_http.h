#pragma once

#include "robot_state.h"
#include <string>

// 定义机器人 HTTP 连接中的 "连接中" 状态
class RobotStateHttpConnecting : public RobotState
{
public:
    // 动态创建状态实例，参数为状态类型 Http_Connecting
    DynamicStateCreate(RobotStateHttpConnecting, RobotStateType::Http_Connecting);

    // 进入状态时的回调函数，状态进入时执行初始化操作
    void OnEnterState() override;

    // 状态更新的回调函数，用于每帧更新状态逻辑，返回新的状态
    RobotStateType OnUpdate() override;
};

// 定义机器人 HTTP 连接中的 "已连接" 状态
class RobotStateHttpConnected : public RobotState
{
public:
    // 动态创建状态实例，参数为状态类型 Http_Connected
    DynamicStateCreate(RobotStateHttpConnected, RobotStateType::Http_Connected);
    
    // 进入状态时的回调函数，状态进入时执行初始化操作
    void OnEnterState() override;

    // 状态更新的回调函数，用于每帧更新状态逻辑，返回新的状态
    RobotStateType OnUpdate() override;
};
