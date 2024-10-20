#pragma once
#include "network.h"
#include "system.h"
#include <string>

class ConnectObj;
class Packet;

class NetworkConnector : public Network,
                         public IAwakeSystem<std::string, int>,
                         public IAwakeSystem<int, int> {
public:
  void Awake(std::string ip, int port) override;
  void Awake(int appType, int appId) override;
  virtual void Update();
  bool IsConnected() const;
  const char *GetTypeName() override;

  static bool IsSingle() { return true; }

protected:
  bool Connect(std::string ip, int port);

private:
  void TryCreateConnectObj();

protected:
  std::string _ip{""};
  int _port{0};
};