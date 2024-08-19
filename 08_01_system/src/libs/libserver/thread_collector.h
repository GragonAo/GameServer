#pragma once
#include "cache_refresh.h"
#include "disposable.h"
#include "thread.h"
#include "thread_type.h"

class Packet;

class ThreadCollector : public IDisposable {
public:
  ThreadCollector(ThreadType threadType, int initNum);
  ~ThreadCollector();
  void CreateThread(int num);

  void Update();
  void Dispose() override;
  bool IsDisposeAll();
  bool IsRunAll();
  bool IsStopAll();
  virtual void HandleMessage(Packet *pPacket);
  virtual void HandlerCreateMessage(Packet *pPacket);

protected:
  ThreadType _threadType;
  CacheRefresh<Thread> _threads;

  size_t _index{0};
};