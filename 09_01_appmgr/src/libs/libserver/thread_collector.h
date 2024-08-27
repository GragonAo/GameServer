#pragma once
#include "cache_refresh.h"
#include "disposable.h"
#include "thread.h"
#include "thread_type.h"

class Packet;

class ThreadCollector : public IDisposable {
public:
  ThreadCollector(ThreadType threadType, int initNum);
  
  void CreateThread(int num);
  void DestroyThread();

  void Update();

  void Dispose() override;
  bool IsDisposeAll();
  bool IsStopAll();

  virtual void HandleMessage(Packet *pPacket);
  virtual void HandlerCreateMessage(Packet *pPacket, bool isToAllThread);

protected:
  ThreadType _threadType;
  CacheRefresh<Thread> _threads;

  uint64 _nextThreadSn{ 0 };
};