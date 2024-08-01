#pragma once

#include "disposable.h"
#include "sn.object.h"
#include "thread_obj.h"
#include <mutex>
#include <thread>

class Packet;

class ThreadObjectList : IDisposable{
public:
  void AddObject(ThreadObject *_obj);
  void Update();
  void AddPacketToList(Packet *pPacket);
  void Dispose() override;
protected:
  std::list<ThreadObject *> _objlist;
  std::mutex _obj_lock;
};

class Thread : public ThreadObjectList, public SnObject{
public:
  Thread();
  void Start();
  void Stop();
  bool IsRun() const;
private:
  bool _isRun;
  std::thread _thread;
};