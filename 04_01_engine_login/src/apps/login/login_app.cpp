#include "login_app.h"
#include "account.h"
void LoginApp::InitApp() {
    AddListenerToThread("127.0.0.1", 2233);
    
    Account* pAccount = new Account();
    _pThreadMgr->AddObjToThread(pAccount);
}