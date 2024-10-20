#pragma once
#include "libserver/state_template.h"
#include "robot_state.h"
#include "robot_state_type.h"
class RobotStateHttpConnecting : public RobotState {
public:
    DynamicStateCreate(RobotStateHttpConnecting,RobotStateType::Http_Connecting);
    void OnEnterState() override;
    RobotStateType OnUpdate() override;
private:
    std::string _ip{""};
    int _port{0};
    std::string _method{""};
};

class RobotStateHttpConnected : public RobotState{
public:
    DynamicStateCreate(RobotStateHttpConnected, RobotStateType::Http_Connected);
    void OnEnterState() override;
    RobotStateType OnUpdate() override;
};