#pragma once

#include "system.h"
#include "thread_type.h"

#include "entity.h"

class Packet;

class ConsoleThreadComponent : public Entity<ConsoleThreadComponent>,
                               public IAwakeSystem<ThreadType> {
public:
  void Awake(ThreadType iType) override;
  void BackToPool() override;

private:
  void HandleCmdShowThreadEntites(Packet *pPacket);
  void HandleCmdThread(Packet* pPacket);
  void HandleCmdThreadPool(Packet* pPacket);
private:
  ThreadType _threadType;

  static std::mutex _show_lock;
};