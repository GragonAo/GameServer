#pragma once
#include "network.h"

class ConnectObj;
class Packet;

class NetworkConnector : public Network {
public:
  virtual bool Connect(std::string ip, int port);
  void Update();
  bool HasRecvData();
  Packet *GetRecvPacket();
  void SendPacket(Packet *pPacket);
  bool IsConnected() const;

private:
  ConnectObj *GetConnectObj();
  void TryCreateConnectObj();

private:
  std::string _ip{""};
  int _port{0};
};