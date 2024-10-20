#pragma once
#include "common.h"
#include "network.h"
#include "system.h"

class NetworkListen : public Network,
                      public IAwakeSystem<std::string, int>,
                      public IAwakeSystem<int, int>,
                      public IAwakeSystem<std::string, int, NetworkType> {
public:
  void Awake(std::string ip, int port) override; // Http连接
  void Awake(int appType, int appId) override;   // Tcp连接
  void Awake(std::string ip, int port, NetworkType iType) override;

  void BackToPool() override;

  virtual void Update();
  const char *GetTypeName() override;
  uint64 GetTypeHashCode() override;
  void CmdShow();

  static bool IsSingle() { return true; }

private:
  void HandleListenKey(Packet *pPacket);

protected:
  virtual int Accept();
#ifdef EPOLL
  virtual void OnEpoll(SOCKET fd, int index) override;
#endif

private:
#ifdef EPOLL
  int _mainSocketEventIndex{-1};
#endif

  SOCKET _masterSocket{INVALID_SOCKET};
};