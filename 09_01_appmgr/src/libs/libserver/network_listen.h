#pragma once
#include "network.h"
#include "network_interface.h"
#include "system.h"

class NetworkListen : public Network,
                      public IAwakeSystem<std::string, int>,
                      public IAwakeSystem<int, int>,
                      public IAwakeSystem<std::string, int, NetworkType> {
public:
  void Awake(std::string ip, int port) override;
  void Awake(int appType, int appId) override;
  void Awake(std::string ip, int port, NetworkType iType) override;

  void Update();
  const char *GetTypeName() override;

  static bool IsSingle() { return true; }

private:
  void HandleDisconnect(Packet *pPacket);

protected:
  virtual int Accept();
};