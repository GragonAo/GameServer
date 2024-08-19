#include "server_app.h"
#include "app_type_mgr.h"
#include "global.h"
#include "console_cmd_thread.h"
#include "thread_mgr.h"
#include "res_path.h"
#include "object_pool_mgr.h"
#include <csignal>
#include <sys/time.h>
#include "console_cmd_pool.h"
#include "network_locator.h"
#include "log4.h"
#include "yaml.h"

ServerApp::ServerApp(APP_TYPE appType,int argc,char* argv[]) {
  _appType = appType;
  _argc = argc;
  _argv = argv;
}

void ServerApp::Initialize(){
  signal(SIGINT,Signalhandler);
  Global::Instance(_appType,1);
  AppTypeMgr::Instance();
  DynamicObjectPoolMgr::Instance();
  ResPath::Instance();
  Log4::Instance(_appType);
  Yaml::Instance();

  _pThreadMgr = ThreadMgr::Instance();
  UpdateTime();

  _pThreadMgr->GetEntitySystem()->AddComponent<NetworkLocator>();
  auto pConsole = _pThreadMgr->GetEntitySystem()->AddComponent<Console>();
  pConsole->Register<ConsoleCmdPool>("pool");
  pConsole->Register<ConsoleCmdThread>("thread");

  _pThreadMgr->InitializeThread();
}

void ServerApp::Dispose(){
  ThreadMgr::DestroyInstance();
  Global::DestroyInstance();
}

void ServerApp::Signalhandler(const int signalValue) {
  switch (signalValue) {
  case SIGTERM:
  case SIGINT:
    Global::GetInstance()->IsStop = true;
    break;
  }

  std::cout << "\nrecv signal. value:" << signalValue
            << " Global IsStop::" << Global::GetInstance()->IsStop << std::endl;
}

void ServerApp::Run() {

  while (!Global::GetInstance()->IsStop) {
    UpdateTime();
    _pThreadMgr->Update();
    DynamicObjectPoolMgr::GetInstance()->Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::cout << "stoping all threads..." << std::endl;
  bool isStop;
  do {
    isStop = _pThreadMgr->IsStopAll();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } while (!isStop);

  std::cout << "disposing all threads..." << std::endl;

  bool isDispose;
  do {
    isDispose = _pThreadMgr->IsDisposeAll();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } while (!isDispose);

  _pThreadMgr->Dispose();

  std::cout << "disposing all pool ...." << std::endl;
  DynamicObjectPoolMgr::GetInstance()->Update();
  DynamicObjectPoolMgr::GetInstance()->Dispose();
  DynamicObjectPoolMgr::DestroyInstance();
}

void ServerApp::UpdateTime() const {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  Global::GetInstance()->TimeTick = tv.tv_sec * 1000 + tv.tv_usec * 0.001;
}