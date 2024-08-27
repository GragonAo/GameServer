#pragma once
#include "libserver/network_connector.h"
#include "libserver/packet.h"
#include "libserver/system.h"
#include "libserver/util_time.h"
#include "robot_state_type.h"

class RobotMgr : public NetworkConnector, IAwakeFromPoolSystem<> {
public:
  void Awake() override;
  void ShowInfo();

  static bool IsSingle() { return true; }

private:
  void HandleRobotState(Packet *pPacket);
  void NofityServer(RobotStateType maxType);

private:
  bool _isChange{false};
  std::map<std::string, RobotStateType> _robots;
};