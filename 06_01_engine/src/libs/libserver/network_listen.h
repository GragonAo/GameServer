#pragma once
#include "system.h"
#include "network.h"

class NetworkListen : public Network,
                      public IUpdateSystem,
                      public IAwakeFromPoolSystem<std::string, int> {
public:
  void AwakeFromPool(std::string ip, int port) override;
  void Update() override;
  const char *GetTypeName() override;

protected:
  virtual int Accept();
};