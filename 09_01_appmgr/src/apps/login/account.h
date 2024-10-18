#pragma once
#include "libserver/entity.h"
#include "libserver/message_system.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/socket_object.h"
#include "libserver/system.h"
#include "login_obj_mgr.h"
#include "json/json.h"

class Account : public Entity<Account>, public IAwakeSystem<> {
public:
  void Awake() override;
  virtual void BackToPool() override;

private:
  void SyncAppInfoToAppMgr();

  void HandleNetworkConnected(Packet *pPacket);
  void HandleNetworkDisconnect(Packet *pPacket);
  void SocketDisconnect(std::string account, NetworkIdentify *pIdentify);

  void HandleHttpOuterResponse(Packet *pPacket);

  void HandleAccountCheck(Packet *pPacket);

  void HandleQueryPlayerListRs(Packet *pPacket);
  void HandleCreatePlayer(Packet *pPacket);
  void HandleCreatePlayerRs(Packet *pPacket);

  Proto::AccountCheckReturnCode ProcessMsg(Json::Value vlaue) const;

private:
  LoginObjMgr _playerMgr;

  std::string _method{""};
  std::string _httpIp{""};
  int _httpPort{0};
};
