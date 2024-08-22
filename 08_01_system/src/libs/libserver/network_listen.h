#pragma once
#include "network.h"
#include "system.h"

class NetworkListen : public Network,
                      public IAwakeSystem<std::string, int> {
public:
  void Awake(std::string ip, int port) override;
  void Update();
  const char *GetTypeName() override;

private:
  void HandleDisconnect(Packet* pPacket);

protected:
  virtual int Accept();
};