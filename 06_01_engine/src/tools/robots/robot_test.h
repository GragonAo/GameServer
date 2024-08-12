#pragma once
#include "libserver/component.h"
#include "libserver/message_system.h"
#include "libserver/packet.h"
#include "libserver/system.h"
#include <chrono>

class RobotTest : public Component<RobotTest>,public IMessageSystem,public IAwakeFromPoolSystem<> {
public:
  void AwakeFromPool() override{};
  void RegisterMsgFunction() override;
  void BackToPool() override{};

private:
  void HandleTestBegin(Packet *pPacket);
  void HandleTestEnd(Packet *pPacket);

private:
  std::chrono::system_clock::time_point _start;
};