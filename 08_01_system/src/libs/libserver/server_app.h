#pragma once
#include "disposable.h"
#include "common.h"
#include "singleton.h"
#include "thread_mgr.h"
#include <signal.h>


class ServerApp : public Singleton<ServerApp>,public IDisposable {
public:
  ServerApp(APP_TYPE appType,int argc,char* argv[]);
  void Initialize();
  void Dispose() override;

  void Run();

  static void Signalhandler(int signalValue);

protected:
  ThreadMgr *_pThreadMgr;
  APP_TYPE _appType;

  int _argc;
  char** _argv;
};