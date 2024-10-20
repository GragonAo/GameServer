#pragma once

#include "libserver/entity.h"
#include "libserver/socket_object.h"
#include "libserver/system.h"

class TestHttpLogin : public NetworkIdentify,
                      public Entity<TestHttpLogin>,
                      public IAwakeSystem<std::string, std::string> {
public:
  void Awake(std::string account, std::string password) override;
  void BackToPool() override;

protected:
  void HandleNetworkDisconnect(TestHttpLogin *pObj, Packet *pPacket);
  void HandleNetworkConnected(TestHttpLogin *pObj, Packet *pPacket);
  void HandleHttpOuterResponse(TestHttpLogin *pObj, Packet *pPacket);

private:
  std::string _account{""};
  std::string _password{""};
  std::string _ip{""};
  int _port{0};
  std::string _mothed{""};
};