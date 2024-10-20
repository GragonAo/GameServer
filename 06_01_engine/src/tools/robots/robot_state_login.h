#pragma once

#include "libserver/state_template.h"
#include "robot_state.h"
#include "robot_state_type.h"

class RobotStateLoginConnecting : public RobotState {
public:
  DynamicStateCreate(RobotStateLoginConnecting, RobotState_Login_Connecting);
  void OnEnterState() override;
  RobotStateType OnUpdate() override;
};

class RobotStateLoginConnected : public RobotState {
public:
  DynamicStateCreate(RobotStateLoginConnected, RobotState_Login_Connected);
  void OnEnterState() override;
};

class RobotStateLoginLogined : public RobotState {
public:
  DynamicStateCreate(RobotStateLoginLogined, RobotState_Login_Logined);
};