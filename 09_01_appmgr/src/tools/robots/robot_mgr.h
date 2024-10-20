#pragma once
#include "libserver/network_connector.h"
#include "libserver/packet.h"
#include "libserver/system.h"
#include "libserver/util_time.h"
#include "robot_state_type.h"
#include <chrono>

class RobotMgr : public Entity<RobotMgr>,public IAwakeSystem<> {
public:
  void Awake() override;
  void BackToPool() override;

  void ShowInfo();
private:
  void HandleRobotState(Packet *pPacket);
  void NofityServer(RobotStateType maxType);

private:
  std::chrono::system_clock::time_point _start;
  bool _isChange{false};
  std::map<std::string, RobotStateType> _robots;
};