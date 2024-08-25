#pragma once

#include "sn_object.h"
#include "thread_type.h"
#include "system_manager.h"
#include <thread>

enum ThreadState {
  Init,
  Run,
  Stop,
  Destroy
};

class Thread : public SystemManager,public SnObject {
public:
  Thread(ThreadType threadType);
  ~Thread();
  
  void Start();
  void DestroyThread();
  void Dispose() override;

  bool IsStop() const;
  bool IsDestroy() const;
private:
  ThreadType _threadType;

  ThreadState _state;
  std::thread _thread;
};