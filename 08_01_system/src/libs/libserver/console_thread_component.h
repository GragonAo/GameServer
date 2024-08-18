#pragma once

#include "system.h"

#include "entity.h"

class Packet;

class ConsoleThreadComponent : public Entity<ConsoleThreadComponent>,
                               public IAwakeFromPoolSystem<> {
public:
  void AwakeFromPool() override;
  void BackToPool() override;

private:
  void HandleCmdShowThreadEntites(Packet *pPacket);
};