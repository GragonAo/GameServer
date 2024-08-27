#pragma once
#include "app_type.h"
#include "common.h"
#include "entity.h"

class Packet;

struct AppInfo {
  APP_TYPE AppType;
  int AppId;
  std::string Ip;
  int Port;
  int Online;
  SOCKET Socket;

  void Parse(Proto::AppInfoSync proto);
};

class SyncComponent : public Entity<SyncComponent> {
public:
  void AppInfoSyncHandle(Packet *pPacket);
  bool GetOnApp(APP_TYPE appType, AppInfo &info);
  void CmdShow();

protected:
  std::map<int, AppInfo> _apps;
};
