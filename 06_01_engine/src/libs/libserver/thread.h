#pragma once

#include "entity_system.h"
#include <thread>

enum ThreadState {
  ThreadState_Init,
  ThreadState_Run,
  ThreadState_Stoped,
};

class Thread : public EntitySystem {
public:
  Thread();
  void Start();
  bool IsStop() const;
  bool IsRun() const;
  bool IsDispose();

private:
  ThreadState _state;
  std::thread _thread;
};