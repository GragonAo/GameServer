#pragma once
#include "libserver/packet.h"
#include "libserver/thread_obj.h"
#include "player_mgr.h"

class Account : public ThreadObject {
public:
  bool Init() override;
  void Update() override;
  void RegisterMsgFunction() override;

private:
  void HandleAccountCheck(Packet *pPacket);
  void HandleAccountCheckToHttpRs(Packet *pPacket);

private:
  PlayerMgr _playerMgr;
};
