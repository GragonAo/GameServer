#include "global_robots.h"
#include "libserver/app_type.h"
#include "libserver/server_app.h"
#include "libserver/thread_mgr.h"
#include "robot_console_login.h"
#include "robot_mgr.h"
#include "robot_console_http.h"
int main(int argc, char *argv[]) {
  GlobalRobots::Instance();
  const APP_TYPE curAPPType = APP_TYPE::APP_ROBOT;
  ServerApp app(curAPPType, argc, argv);
  app.Initialize();

  auto pThreadMgr = ThreadMgr::GetInstance();

  auto pConsole = pThreadMgr->GetEntitySystem()->GetComponent<Console>();
  pConsole->Register<RobotConsoleLogin>("login");
  pConsole->Register<RobotConsoleHttp>("http");

  pThreadMgr->GetEntitySystem()->AddComponent<RobotMgr>();

  // connector
  pThreadMgr->CreateComponent<NetworkConnector>(
      ConnectThread, false, (int)NetworkType::TcpConnector, 0);
  pThreadMgr->CreateComponent<NetworkConnector>(
      ConnectThread, false, (int)NetworkType::HttpConnector, 0);

  app.Run();
  app.Dispose();

  return 0;
}