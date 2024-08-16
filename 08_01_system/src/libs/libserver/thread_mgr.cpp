#include "thread_mgr.h"
#include "entity_system.h"
#include "system_manager.h"
#include "thread.h"
#include <mutex>
// 构造函数，初始化ThreadMgr
ThreadMgr::ThreadMgr() {}

// 启动所有线程
void ThreadMgr::StartAllThread() {
  auto iter = _threads.begin();
  while (iter != _threads.end()) {
    (*iter)->Start();
    ++iter;
  }
}

void ThreadMgr::Update() {
  _create_lock.lock();
  if (_createPackets.CanSwap()) {
    _createPackets.Swap();
  }
  _create_lock.unlock();

  auto pList = _createPackets.GetReaderCache();
  for (auto iter = pList->begin(); iter != pList->end(); ++iter) {
    const auto packet = (*iter);
    if (_threads.size() > 0) {
      if (_threadIndex >= _threads.size()) {
        _threadIndex = 0;
      }
      _threads[_threadIndex]->GetMessageSystem()->AddPacketToList(packet);
      _threadIndex++;
    } else {
      // 单线程
      GetMessageSystem()->AddPacketToList(packet);
    }
  }
  pList->clear();

  SystemManager::Update();
}

void ThreadMgr::CreateThread() { _threads.emplace_back(new Thread()); }

bool ThreadMgr::IsStopAll() {
  for (auto iter = _threads.begin(); iter != _threads.end(); ++iter) {
    if (!(*iter)->IsStop()) {
      return false;
    }
  }
  return true;
}

bool ThreadMgr::IsDisposeAll() {
  for (auto iter = _threads.begin(); iter != _threads.end(); ++iter) {
    if (!(*iter)->IsDispose()) {
      return false;
    }
  }
  return true;
}

void ThreadMgr::Dispose() {
  SystemManager::Dispose();

  auto iter = _threads.begin();
  while (iter != _threads.end()) {
    Thread *pThread = *iter;
    delete pThread;
    ++iter;
  }
  _threads.clear();
}

// 分发数据包
void ThreadMgr::DispatchPacket(Packet *pPacket) {
  GetMessageSystem()->AddPacketToList(pPacket);
  for (auto iter = _threads.begin(); iter != _threads.end(); iter++) {
    (*iter)->GetMessageSystem()->AddPacketToList(pPacket);
  }
}
