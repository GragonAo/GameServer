#pragma once

#include "sn_object.h"
#include "thread_type.h"
#include "system_manager.h"
#include <thread>

enum ThreadState {
  ThreadState_Init,
  ThreadState_Run,
  ThreadState_Stoped,
};

class Thread : public SystemManager,public SnObject {
public:
  Thread(ThreadType threadType);
  void BackToPool();
  
  void Start();
  bool IsStop() const;
  bool IsRun() const;
  bool IsDispose();

private:
  ThreadType _threadType;

  ThreadState _state;
  std::thread _thread;
};