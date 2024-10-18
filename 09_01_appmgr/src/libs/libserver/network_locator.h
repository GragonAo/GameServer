#pragma once

#include "network_help.h"
#include "network_type.h"
#include "socket_object.h"
#include "network.h"
#include "system.h"

class NetworkLocator : public Component<NetworkLocator>,
                       public IAwakeSystem<> {
public:
  void Awake() override;
  void BackToPool() override;
  
  //connect
  void AddConnectorLocator(INetwork* pNetwork,NetworkType networkType);
  void AddNetworkIdentify(uint64 appKey,SocketKey socket,ObjectKey  objKey);
  void RemoveNetworkIdentify(uint64 appKey);


  INetwork* GetConnector(NetworkType networkType);
  NetworkIdentify GetNetworkIdentify(const APP_TYPE appType,const int appId);

  //listen
  void AddListenLocator(INetwork *pNetwork, NetworkType networkType);
  INetwork *GetListen(NetworkType networkType);
private:
  std::mutex _lock;

  std::map<uint64,NetworkIdentify> _netIdentify;
  std::map<NetworkType,INetwork*> _connectors;
  std::map<NetworkType, INetwork*> _listens;
};