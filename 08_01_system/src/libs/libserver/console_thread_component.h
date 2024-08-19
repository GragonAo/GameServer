#pragma once

#include "system.h"
#include "thread_type.h"

#include "entity.h"

class Packet;

class ConsoleThreadComponent : public Entity<ConsoleThreadComponent>,
                               public IAwakeFromPoolSystem<ThreadType> {
public:
  void AwakeFromPool(ThreadType iType) override;
  void BackToPool() override;

private:
  void HandleCmdShowThreadEntites(Packet *pPacket);

private:
  ThreadType _threadType;
};