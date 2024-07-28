#pragma once

#include "sn.object.h"
#include "thread_obj.h"
#include <mutex>
#include <thread>

class Packet;

class ThreadObjectList {
public:
  void AddObject(ThreadObject *_obj);
  void Update();
  void AddPacketToList(Packet *pPacket);

protected:
  std::list<ThreadObject *> _objlist;
  std::mutex _obj_lock;
};

class Thread : public ThreadObjectList, public SnObject, public IDisposable {
public:
  Thread();
  void Start();
  void Stop();
  bool IsRun() const;
  void Dispose() override;

private:
  bool _isRun;
  std::thread _thread;
};