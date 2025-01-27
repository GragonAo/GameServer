#include "entity_system.h"
#include "create_component.h"
#include "component.h"
#include "message_system.h"
#include "packet.h"
#include <mutex>

EntitySystem::~EntitySystem() {}

void EntitySystem::InitComponent() { AddComponent<CreateComponentC>(); }

void EntitySystem::Update() {
  UpdateMessage();
  auto iter = _updateSystems.begin();
  while (iter != _updateSystems.end()) {
    auto pComponent = dynamic_cast<IComponent *>(*iter);
    if (!pComponent->IsActive()) {
      _objSystems.erase(pComponent->GetSN());
      iter = _updateSystems.erase(iter);
      pComponent->ComponentBackToPool();
    } else {
      (*iter)->Update();
      ++iter;
    }
  }
}

void EntitySystem::UpdateMessage() {
  _packet_lock.lock();
  if (_cachePakcet.CanSwap()) {
    _cachePakcet.Swap();
  }
  _packet_lock.unlock();

  auto pMsgList = _cachePakcet.GetReaderCache();
  for (auto itMsg = pMsgList->begin(); itMsg != pMsgList->end(); ++itMsg) {
    auto pPacket = (*itMsg);
    for (auto iter = _messageSystem.begin(); iter != _messageSystem.end();
         ++iter) {
      auto pObj = (*iter);
      if (pObj->IsFollowMsgId(pPacket))
        pObj->ProcessPacket(pPacket);
    }
  }
  pMsgList->clear();
}

void EntitySystem::AddPacketToList(Packet *pPacket) {
  std::lock_guard<std::mutex> guard(_packet_lock);
  _cachePakcet.GetWriterCache()->emplace_back(pPacket);
}

void EntitySystem::AddToSystem(IComponent *pComponent) {
  pComponent->SetEntitySystem(this);
  _objSystems[pComponent->GetSN()] = pComponent;

  const auto objUpdate = dynamic_cast<IUpdateSystem *>(pComponent);
  if (objUpdate != nullptr)
    _updateSystems.emplace_back(objUpdate);
  const auto objMsg = dynamic_cast<IMessageSystem *>(pComponent);
  if (objMsg != nullptr) {
    objMsg->RegisterMsgFunction();
    _messageSystem.emplace_back(objMsg);
  }
}

void EntitySystem::Dispose() {
  for (auto &iter : _updateSystems) {
    auto pComponent = dynamic_cast<IComponent *>(iter);
    pComponent->BackToPool();
  }
  _updateSystems.clear();
}