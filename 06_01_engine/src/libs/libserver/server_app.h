#pragma once
#include "disposable.h"
#include "common.h"
#include "thread_mgr.h"
#include <signal.h>

template <class APPClass> inline int MainTemplate() {
  APPClass *pApp = new APPClass();
  pApp->InitApp();
  pApp->Run();
  delete pApp;
  return 0;
}
class ServerApp : public IDisposable {
public:
  ServerApp(APP_TYPE appType);
  ~ServerApp();
  virtual void InitApp() = 0;
  void Dispose() override;
  void Run() const;
  void UpdateTime() const;

  static void Signalhandler(int signalValue);

protected:
  ThreadMgr *_pThreadMgr;
  APP_TYPE _appType;
};