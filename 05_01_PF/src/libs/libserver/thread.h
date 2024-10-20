#pragma once

#include "cache_refresh.h"
#include "cache_swap.h"
#include "disposable.h"
#include "sn.object.h"
#include "thread_obj.h"
#include <mutex>
#include <thread>

enum ThreadState {
  ThreadState_Init,
  ThreadState_Run,
  ThreadState_Stoped,
};

class ThreadObjectList : IDisposable {
public:
  void AddObject(ThreadObject *_obj);
  void Update();
  void AddPacketToList(Packet *pPacket);
  void Dispose() override;

protected:
  CacheRefresh<ThreadObject> _objlist;
  std::mutex _obj_lock;

  std::mutex _packet_lock;
  CacheSwap<Packet> _cachePackets;
};

class Thread : public ThreadObjectList, public SnObject {
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