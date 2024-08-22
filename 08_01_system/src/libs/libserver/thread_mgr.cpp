#include "thread_mgr.h"
#include "console.h"
#include "console_cmd_thread.h"
#include "log4.h"
#include "message_system.h"
#include "network_locator.h"
#include "packet.h"
#include "res_path.h"
#include "yaml.h"

#include "log4_help.h"
#include "thread_collector_exclusive.h"

#include <iostream>
// 构造函数，初始化ThreadMgr
ThreadMgr::ThreadMgr() {}

void ThreadMgr::InitializeThread() {
  const auto pConfig = GetEntitySystem()->GetComponent<Yaml>()->GetConfig(
      Global::GetInstance()->GetCurAppType());
  auto pAppConfig = dynamic_cast<AppConfig *>(pConfig);

  if (pAppConfig->LogicThreadNum > 0) {
    CreateThread(LogicThread, pAppConfig->LogicThreadNum);
  }
  if (pAppConfig->MysqlThreadNum > 0) {
    CreateThread(MysqlThread, pAppConfig->MysqlThreadNum);
  }
}

void ThreadMgr::CreateThread(ThreadType iType, int num) {
  const auto pConfig = GetEntitySystem()->GetComponent<Yaml>()->GetConfig(
      Global::GetInstance()->GetCurAppType());
  auto pAppConfig = dynamic_cast<AppConfig *>(pConfig);
  if (pAppConfig->LogicThreadNum == 0 && pAppConfig->MysqlThreadNum == 0)
    return;
  LOG_DEBUG("Initialize thread: " << GetThreadTypeName(iType)
                                  << " thread num: " << num);

  auto iter = _threads.find(iType);
  if (iter == _threads.end()) {
    if (iType == MysqlThread) {
      _threads[iType] = new ThreadCollectorExclusive(iType, num);
    } else {
      _threads[iType] = new ThreadCollector(iType, num);
    }
  } else {
    _threads[iType]->CreateThread(num);
  }
}

void ThreadMgr::InitializeGloablComponent(APP_TYPE ppType, int appId) {
  GetEntitySystem()->AddComponent<ResPath>();
  GetEntitySystem()->AddComponent<Log4>(ppType);
  GetEntitySystem()->AddComponent<Yaml>();
  GetEntitySystem()->AddComponent<NetworkLocator>();

  auto pConsole = GetEntitySystem()->AddComponent<Console>();
  pConsole->Register<ConsoleCmdThread>("thread");

  InitComponent(ThreadType::MainThread);
}

void ThreadMgr::Update() {
  UpdateCreatePacket();
  UpdateDispatchPacket();
  SystemManager::Update();
}

void ThreadMgr::UpdateCreatePacket() {
  _create_lock.lock();
  if (_createPackets.CanSwap()) {
    _createPackets.Swap();
  }
  _create_lock.unlock();

  auto pList = _createPackets.GetReaderCache();
  for (auto iter = pList->begin(); iter != pList->end(); ++iter) {
    const auto packet = (*iter);
    if (_threads.size() > 0) {
      auto pCreateProto = packet->ParseToProto<Proto::CreateComponent>();
      auto threadType = (ThreadType)(pCreateProto.thread_type());
      if (_threads.find(threadType) == _threads.end()) {
        LOG_ERROR("can't find threadtype: " << GetThreadTypeName(threadType));
        continue;
      }
      auto pThreadCollector = _threads[threadType];
      pThreadCollector->HandlerCreateMessage(packet);
    } else {
      // 单线程
      GetMessageSystem()->AddPacketToList(packet);
    }
  }
  pList->clear();
}

void ThreadMgr::UpdateDispatchPacket() {
  _packet_lock.lock();
  if (_packets.CanSwap()) {
    _packets.Swap();
  }
  _packet_lock.unlock();

  auto pList = _packets.GetReaderCache();
  for (auto iter = pList->begin(); iter != pList->end(); ++iter) {
    auto pPakcet = (*iter);
    GetMessageSystem()->AddPacketToList(pPakcet);
    for (auto iter = _threads.begin(); iter != _threads.end(); ++iter) {
      iter->second->HandleMessage(pPakcet);
    }
  }
  pList->clear();
}

bool ThreadMgr::IsStopAll() {
  for (auto iter = _threads.begin(); iter != _threads.end(); ++iter) {
    if (!iter->second->IsStopAll()) {
      return false;
    }
  }
  return true;
}

bool ThreadMgr::IsDisposeAll() {
  for (auto iter = _threads.begin(); iter != _threads.end(); ++iter) {
    if (!iter->second->IsDisposeAll()) {
      return false;
    }
  }
  return true;
}

void ThreadMgr::Dispose() {
  SystemManager::Dispose();

  for (auto iter = _threads.begin(); iter != _threads.end(); ++iter) {
    auto pCollector = iter->second;
    delete pCollector;
  }
  _threads.clear();
}

// 分发数据包
void ThreadMgr::DispatchPacket(Packet *pPacket) {
  std::lock_guard<std::mutex> guard(_packet_lock);
  _packets.GetWriterCache()->emplace_back(pPacket);
}
