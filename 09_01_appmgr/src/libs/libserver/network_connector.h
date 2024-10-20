#pragma once
#include "app_type.h"
#include "cache_refresh.h"
#include "disposable.h"
#include "sn_object.h"
#include "socket_object.h"
#include "network.h"
#include "system.h"
#include <string>

class ConnectObj;
class Packet;

struct ConnectDetail : public SnObject, public IDisposable {
public:
  ConnectDetail(ObjectKey key, std::string ip, int port) {
    Key = std::move(key);
    Ip = std::move(ip);
    Port = port;
  };

  void Dispose() override {}

  std::string Ip{""};
  int Port{0};
  ObjectKey Key{ObjectKeyType::None, {0, ""}};
};

class NetworkConnector : public Network, public IAwakeSystem<int, int> {
public:
  void Awake(int iType, int mixConnectAppType) override;
  virtual void Update();

  const char *GetTypeName() override;
  uint64 GetTypeHashCode() override;
  bool Connect(ConnectDetail *pDetail);

private:
  void HandleNetworkConnect(Packet *pPacket);
  void CreateConnector(APP_TYPE appType, int appId);

protected:
  CacheRefresh<ConnectDetail> _connecting;
};