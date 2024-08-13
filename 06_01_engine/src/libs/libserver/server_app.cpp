#include "server_app.h"
#include "app_type_mgr.h"
#include "res_path.h"
#include "object_pool_mgr.h"
#include <sys/time.h>
#include "console_cmd_pool.h"
#include "network_locator.h"
#include "yaml.h"
ServerApp::ServerApp(APP_TYPE appType) {

  signal(SIGINT, Signalhandler);

  _appType = appType;
  Global::Instance(_appType,1);

  AppTypeMgr::Instance();
  ResPath::Instance();
  Yaml::Instance();
  DynamicObjectPoolMgr::Instance();

  _pThreadMgr = ThreadMgr::Instance();
  UpdateTime();

  _pThreadMgr->AddComponent<NetworkLocator>();
  auto pConsole = _pThreadMgr->AddComponent<Console>();
  pConsole->Register<ConsoleCmdPool>("pool");

  const auto pLoginConfig = dynamic_cast<AppConfig*>(Yaml::GetInstance()->GetConfig(appType));
  for (int i = 0; i < pLoginConfig->ThreadNum; i++) {
    _pThreadMgr->CreateThread();
  }
  _pThreadMgr->InitComponent();
  _pThreadMgr->StartAllThread();
  
}

ServerApp::~ServerApp() { _pThreadMgr->DestroyInstance(); }

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

void ServerApp::Dispose() { _pThreadMgr->Dispose(); }

void ServerApp::Run() const {

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

  Global::DestroyInstance();
  ThreadMgr::DestroyInstance();
}
void ServerApp::UpdateTime() const {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  Global::GetInstance()->TimeTick = tv.tv_sec * 1000 + tv.tv_usec * 0.001;
}