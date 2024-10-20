#include "server_app.h"
#include "app_type.h"
#include "global.h"
#include "object_pool_packet.h"
#include "thread_mgr.h"
#include <csignal>

ServerApp::ServerApp(APP_TYPE appType, int argc, char *argv[]) {
  _appType = appType;
  _argc = argc;
  _argv = argv;
}

void ServerApp::Initialize() {

    std::cout << "\ncommand arguments:" << std::endl;
    for (auto count = 0; count < _argc; count++)
        std::cout << "  argv[" << count << "]   " << _argv[count] << std::endl;

    for (int argIdx = 1; argIdx < _argc; ++argIdx)
    {
        std::string cmd = _argv[argIdx];
        std::string findcmd = "-sid=";
        std::string::size_type fi1 = cmd.find(findcmd);
        if (fi1 != std::string::npos)
        {
            cmd.erase(fi1, findcmd.size());
            _appId = std::stoi(cmd);
            break;
        }
    }
  signal(SIGINT, Signalhandler);

  Global::Instance(_appType, _appId);
  DynamicPacketPool::Instance();

  _pThreadMgr = ThreadMgr::Instance();
  _pThreadMgr->InitializeGloablComponent(_appType, _appId);

  _pThreadMgr->InitializeThread();
}

void ServerApp::Dispose() {
  DynamicPacketPool::GetInstance()->Dispose();
  DynamicPacketPool::DestroyInstance();
  ThreadMgr::DestroyInstance();
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
  log4cplus::initialize();
  auto pGlobal = Global::GetInstance();

  while (!pGlobal->IsStop) {
    pGlobal->UpdateTime();
    _pThreadMgr->Update();
    DynamicPacketPool::GetInstance()->Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::cout << "stoping all threads..." << std::endl;
  bool isStop;
  do {
    isStop = _pThreadMgr->IsStopAll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (!isStop);

  std::cout << "disposing all threads..." << std::endl;
  _pThreadMgr->DestroyThread();

  std::cout << "disposing all pool ...." << std::endl;
  bool isDispose;
  do {
    isDispose = _pThreadMgr->IsDisposeAll();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } while (!isDispose);

  _pThreadMgr->Dispose();

  log4cplus::deinitialize();
}