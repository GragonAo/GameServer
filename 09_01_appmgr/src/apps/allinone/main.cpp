
#include "appmgr/appmgr.h"
#include "libserver/thread_mgr.h"
#include "libserver/server_app.h"
#include "libserver/thread_type.h"
#include "login/login.h"
#include "dbmgr/dbmgr.h"
#include "libserver/network_listen.h"

int main(int argc, char* argv[])
{
    const APP_TYPE curAppType = APP_TYPE::APP_ALLINONE;

    ServerApp app(curAppType, argc, argv);
    app.Initialize();

    auto pThreadMgr = ThreadMgr::GetInstance();

    // appmgr
    InitializeComponentAppMgr(pThreadMgr);
    
    // dbmgr
    InitializeComponentDBMgr(pThreadMgr);

    // login
    InitializeComponentLogin(pThreadMgr);

    auto pGlobal = Global::GetInstance();
    pThreadMgr->CreateThread(ListenThread, 1);
    pThreadMgr->CreateComponent<NetworkListen>(ListenThread, false, (int)pGlobal->GetCurAppType(), pGlobal->GetCurAppId());

    app.Run();
    app.Dispose();

    return 0;
}
