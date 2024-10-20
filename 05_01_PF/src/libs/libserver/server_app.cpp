#include "server_app.h"
#include "network_listen.h"
#include <chrono>
#include <thread>

ServerApp::ServerApp(APP_TYPE appType) {

  signal(SIGINT, Signalhandler);

  _appType = appType;
  Global::Instance();
  _pThreadMgr = ThreadMgr::Instance();
  UpdateTime();
  for (int i = 0; i < 3; i++) {
    _pThreadMgr->NewThread();
  }
  _pThreadMgr->StartAllThread();
}
ServerApp::~ServerApp() { _pThreadMgr->DestroyInstance(); }

void ServerApp::Signalhandler(const int signalValue)
{
    switch (signalValue)
    {
    case SIGTERM:
    case SIGINT:
        Global::GetInstance()->IsStop = true;
        break;
}

    std::cout << "\nrecv signal. value:" << signalValue << " Global IsStop::" << Global::GetInstance()->IsStop << std::endl;
}

void ServerApp::Dispose() { _pThreadMgr->Dispose(); }
void ServerApp::Run() const {
while (!Global::GetInstance()->IsStop)
    {
        UpdateTime();        
        _pThreadMgr->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // ֹͣ�����߳�
    std::cout << "stoping all threads..." << std::endl;
    bool isStop;
    do
    {
        isStop = _pThreadMgr->IsStopAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (!isStop);

    // �ͷ������߳���Դ
    std::cout << "disposing all threads..." << std::endl;
   
    // 1.���߳���Դ
    bool isDispose;
    do
    {
        isDispose = _pThreadMgr->IsDisposeAll();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (!isDispose);

    // 2.���߳���Դ
    _pThreadMgr->Dispose();
}
void ServerApp::UpdateTime() const {
  auto timeVlaue = std::chrono::time_point_cast<std::chrono::microseconds>(
      std::chrono::system_clock::now());
  Global::GetInstance()->TimeTick = timeVlaue.time_since_epoch().count();

  // auto tt = std::chrono::system_clock::to_time_t(timeVlaue);
  // struct tm *ptm = localtime(&tt);
  // Global::GetInstance()->YearDay = ptm->tm_year;
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