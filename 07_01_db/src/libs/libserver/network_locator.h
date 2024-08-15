#pragma once

#include "common.h"
#include "component.h"
#include "network.h"
#include "network_interface.h"
#include "system.h"
#include <tuple>

class NetworkLocator : public Component<NetworkLocator>,
                       public IAwakeFromPoolSystem<> {
public:
  void AwakeFromPool() override{};
  void BackToPool() override;
  void AddConnectorLocator(INetwork *pNetwork, APP_TYPE aooType, int appId);
  void AddListenLocator(INetwork *pNetwork, NetworkType networkType);

  INetwork *GetListen(NetworkType networkType);
  INetwork *GetNetworkConnector(const SOCKET socket);
  INetwork *GetNetworkConnector(const APP_TYPE appType, const int appId);
  std::tuple<APP_TYPE, int> GetNetworkConnectorInfo(const SOCKET socket);

  std::list<INetwork *> GetNetworks(const APP_TYPE appType);

  APP_TYPE GetNetworkAppType(const int socket);
  int GetNetworkAppId(const SOCKET socket);

private:
  std::mutex _lock;
  std::map<APP_TYPE, std::map<int, INetwork *>> _connectors;

  std::map<NetworkType, INetwork *> _listens;
};