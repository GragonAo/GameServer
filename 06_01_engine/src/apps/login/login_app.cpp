#include "login_app.h"
#include "account.h"
#include "libserver/network_listen.h"
#include "robots/robot_test.h"
void LoginApp::InitApp() {
    _pThreadMgr->CreateComponent<NetworkListen>(std::string("127.0.0.1"), 2233);
    _pThreadMgr->CreateComponent<RobotTest>();
    _pThreadMgr->CreateComponent<Account>();
}