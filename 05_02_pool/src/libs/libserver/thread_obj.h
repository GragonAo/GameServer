#pragma once

#include "disposable.h"
#include "sn_object.h"
#include "message_list.h"
class Thread;
class ThreadObject : public MessageList,public SnObject {
public:
  virtual bool Init() = 0;
  virtual void RegisterMsgFunction() = 0;
  virtual void Update() = 0;

  void SetThread(Thread* pThread);
  Thread* GetThread() const;
  bool IsActive() const;
  virtual void Dispose() override;

protected:
  bool _active{true};
  Thread* _pThread{nullptr};
};