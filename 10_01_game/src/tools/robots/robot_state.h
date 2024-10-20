#pragma once
#include "libserver/state_template.h"
#include "libserver/robot_state_type.h"

class Robot;

// `RobotState` 继承自状态模板，用于管理 `RobotStateType` 与 `Robot` 的状态机
class RobotState : public StateTemplate<RobotStateType, Robot>
{
public:
    // 更新状态的方法，返回新的状态类型
    RobotStateType Update() override;
    
    // 可以被子类重写的状态更新逻辑
    virtual RobotStateType OnUpdate()
    {
        return GetState();
    }

    // 进入状态时的操作
    void EnterState() override;
    
    // 子类可以重写该方法定义进入状态时的额外行为
    virtual void OnEnterState()
    {
    }

    // 离开状态时的操作
    void LeaveState() override;

    // 子类可以重写该方法定义离开状态时的额外行为
    virtual void OnLeaveState()
    {
    }
};
