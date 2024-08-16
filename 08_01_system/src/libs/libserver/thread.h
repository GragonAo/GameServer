#pragma once

#include "system_manager.h"
#include <thread>

enum ThreadState {
  ThreadState_Init,
  ThreadState_Run,
  ThreadState_Stoped,
};

class Thread : public SystemManager {
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