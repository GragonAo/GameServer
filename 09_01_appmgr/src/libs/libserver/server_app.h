#pragma once
#include "common.h"
#include "disposable.h"
#include "singleton.h"
#include "thread_mgr.h"
#include <signal.h>

class ServerApp : public Singleton<ServerApp>, public IDisposable {
public:
  ServerApp(APP_TYPE appType, int argc, char *argv[]);
  void Initialize();
  void Dispose() override;

  void Run();

  static void Signalhandler(int signalValue);

protected:
  ThreadMgr *_pThreadMgr{nullptr};
  APP_TYPE _appType{APP_TYPE::APP_None};
  int _appId{0};

  int _argc;
  char **_argv;
};