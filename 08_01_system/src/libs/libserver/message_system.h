#pragma once

#include "cache_swap.h"
#include "component.h"
#include "system.h"
#include <mutex>

class Packet;
class EntitySystem;
class IComponent;
class SystemManager;

class MessageSystem : virtual public ISystem {
public:
  MessageSystem(SystemManager *pMgr);
  void Dispose() override;
  void Update(EntitySystem *pEntities) override;
  void AddPacketToList(Packet *pPacket);

private:
  static void Process(Packet *pPacket, std::map<uint64, IComponent *> &lists);

private:
  std::mutex _packet_lock;
  CacheSwap<Packet> _cachePacket;
  SystemManager *_systemMgr{nullptr};
};