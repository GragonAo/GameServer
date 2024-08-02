#include "robots_app.h"
#include "robot_console.h"
#include "robot_mgr.h"

void RobotsApp::InitApp() {
  RobotMgr *mgr = new RobotMgr();
  _pThreadMgr->AddObject(mgr);

  RobotConsole *pConsole = new RobotConsole();
  _pThreadMgr->AddObjToThread(pConsole);
}