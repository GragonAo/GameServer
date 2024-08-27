#pragma once
#include "libserver/entity.h"
#include "libserver/system.h"

class ConsoleAppComponent : public Entity<ConsoleAppComponent>,
                            public IAwakeSystem<> {
public:
  void Awake();
  void BackToPool();

private:
  void HandleCmdApp(Packet *pPacket);
  void HandleCmdAppInfo();
};