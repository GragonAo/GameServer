#pragma once
#include "libserver/entity.h"
#include "libserver/message_system.h"
#include "libserver/packet.h"
#include "libserver/system.h"
#include <chrono>

class RobotTest : public Entity<RobotTest>, public IAwakeFromPoolSystem<> {
public:
  void AwakeFromPool() override;
  void BackToPool() override {};

private:
  void HandleTestBegin(Packet *pPacket);
  void HandleTestEnd(Packet *pPacket);

private:
  std::chrono::system_clock::time_point _start;
};