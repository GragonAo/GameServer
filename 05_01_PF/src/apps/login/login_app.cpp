#include "login_app.h"
#include "account.h"
#include "libserver/console.h"
void LoginApp::InitApp() {
    AddListenerToThread("127.0.0.1", 2233);
    
    Account* pAccount = new Account();
    _pThreadMgr->AddObjToThread(pAccount);

    Console* pConsole = new Console();
    _pThreadMgr->AddObjToThread(pConsole);
}