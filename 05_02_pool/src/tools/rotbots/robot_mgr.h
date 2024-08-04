#pragma once
#include "libserver/network_connector.h"
#include "libserver/packet.h"
#include "libserver/util_time.h"
#include "robot_state_type.h"

class RobotMgr : public NetworkConnector {
public:
  bool Init() override;
  void Update() override;
  void RegisterMsgFunction() override;

  void ShowInfo();

private:
  void HandleRobotState(Packet *pPacket);
  void NofityServer(RobotStateType maxType);

private:
  bool _isChange{false};
  std::map<std::string, RobotStateType> _robots;
  timeutil::Time _nextShowInfoTime{0};
};