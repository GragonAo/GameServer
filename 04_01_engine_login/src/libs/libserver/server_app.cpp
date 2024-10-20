#include "server_app.h"
#include "network_listen.h"
#include <chrono>
#include <thread>

ServerApp::ServerApp(APP_TYPE appType) {
  _appType = appType;
  Global::Instance();
  _pThreadMgr = ThreadMgr::Instance();
  UpdateTime();
  for (int i = 0; i < 3; i++) {
    _pThreadMgr->NewThread();
  }
}
ServerApp::~ServerApp() { _pThreadMgr->DestroyInstance(); }
void ServerApp::Dispose() { _pThreadMgr->Dispose(); }
void ServerApp::StartAllThread() const { _pThreadMgr->StartAllThread(); }
void ServerApp::Run() const {
  bool isRun = true;
  while (isRun) {
    UpdateTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    isRun = _pThreadMgr->IsGameLoop();
  }
}
void ServerApp::UpdateTime() const {
  auto timeVlaue = std::chrono::time_point_cast<std::chrono::microseconds>(
      std::chrono::system_clock::now());
  Global::GetInstance()->TimeTick = timeVlaue.time_since_epoch().count();
  auto tt = std::chrono::system_clock::to_time_t(timeVlaue);
  struct tm *ptm = localtime(&tt);
  Global::GetInstance()->YearDay = ptm->tm_year;
}
bool ServerApp::AddListenerToThread(std::string ip, int port) const {
  NetworkListen *pListener = new NetworkListen();
  if (!pListener->Listen(ip, port)) {
    delete pListener;
    return false;
  }
  _pThreadMgr->AddNetworkToThread(APP_Listen,pListener);
  return true;
}