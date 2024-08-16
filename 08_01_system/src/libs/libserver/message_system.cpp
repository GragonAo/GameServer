#include "message_system.h"
#include "entity_system.h"
#include "message_component.h"
#include <mutex>

MessageSystem::MessageSystem(SystemManager *pMgr) { _systemMgr = pMgr; }

void MessageSystem::Dispose() {}

void MessageSystem::AddPacketToList(Packet *pPacket) {
  std::lock_guard<std::mutex> guard(_packet_lock);
  _cachePacket.GetWriterCache()->emplace_back(pPacket);
}

void MessageSystem::Update(EntitySystem *pEntities) {
  _packet_lock.lock();
  if (_cachePacket.CanSwap())
    _cachePacket.Swap();
  _packet_lock.unlock();

  if (_cachePacket.GetReaderCache()->size() == 0)
    return;

  auto pCollections = pEntities->GetComponentCollections<MessageComponent>();
  if (pCollections == nullptr) {
    _cachePacket.GetReaderCache()->clear();
    return;
  }
  pCollections->Swap();

  auto lists = pCollections->GetAll();
  auto packetLists = _cachePacket.GetReaderCache();
  for (auto iter = packetLists->begin(); iter != packetLists->end(); ++iter) {
    Process((*iter), lists);
  }
  packetLists->clear();
}

void MessageSystem::Process(Packet *pPacket,
                            std::map<uint64, IComponent *> &lists) {
  for (auto iter = lists.begin(); iter != lists.end(); ++iter) {
    MessageComponent *pMsgComponent =
        static_cast<MessageComponent *>(iter->second);
    if (pMsgComponent->IsFollowMsgId(pPacket)) {
      pMsgComponent->ProcessPacket(pPacket);
    }
  }
}
