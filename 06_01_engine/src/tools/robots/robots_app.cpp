#include "robots_app.h"
#include "libserver/console.h"
#include "robot_console_login.h"
#include "robot_mgr.h"

void RobotsApp::InitApp() {
  auto pConsole = _pThreadMgr->GetComponent<Console>();
  pConsole->Register<RobotConsoleLogin>("login");

  _pThreadMgr->AddComponent<RobotMgr>();
}