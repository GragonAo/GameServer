#pragma once
#include "libserver/network_connector.h"
#include "libserver/state_template.h"
#include "robot_state.h"
#include "robot_state_type.h"
class Robot : public NetworkConnector,
              public StateTemplateMgr<RobotStateType, RobotState, Robot> {
public:
  explicit Robot(std::string account);
  bool Init() override;
  void Update() override;
  std::string GetAccount() const;
  void SendMsgAccountCheck();
  void RegisterMsgFunction()override;
protected:
  void RegisterState() override;

private:
  void HandleAccountCheckRs(Robot *pRobot, Packet *pPacket);

private:
  std::string _account;
};