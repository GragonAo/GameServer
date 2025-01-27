#pragma once

#include "component.h"
#include "socket_object.h"
#include "object_pool.h"
#include "packet.h"
#include "singleton.h"

#include <mutex>

class DynamicPacketPool : public DynamicObjectPool<Packet>,
                          public Singleton<DynamicPacketPool> {

public:
  Packet *MallocPacket(Proto::MsgId msgId, NetworkIdentify* pIdentify);
  virtual void Update() override;
  virtual void FreeObject(IComponent* pObj) override;
  virtual void Show() override;
private:
  std::mutex _packet_lock;
};