#pragma once

#include "entity.h"
#include "message_system.h"
#include "system.h"

class Packet;

class CreateComponentC : public Entity<CreateComponentC>,
                         public IAwakeSystem<> {
public:
  void Awake() override;
  void BackToPool() override;

private:
  void HandleCreateComponent(Packet *pPacket) const;
  void HandleRemoveComponent(Packet *pPacket);
};