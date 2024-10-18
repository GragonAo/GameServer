
#include "appmgr/appmgr.h"
#include "dbmgr/dbmgr.h"
#include "libserver/component_help.h"
#include "libserver/network_connector.h"
#include "libserver/network_listen.h"
#include "libserver/network_type.h"
#include "libserver/server_app.h"
#include "libserver/thread_mgr.h"
#include "libserver/thread_type.h"
#include "login/login.h"

int main(int argc, char *argv[]) {
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
  pThreadMgr->CreateComponent<NetworkListen>(ListenThread, false,
                                             (int)pGlobal->GetCurAppType(),
                                             pGlobal->GetCurAppId());

  const auto pYaml = ComponentHelp::GetYaml();
  const auto pCommonConfig =
      pYaml->GetIPEndPoint(pGlobal->GetCurAppType(), pGlobal->GetCurAppId());
  pThreadMgr->CreateComponent<NetworkListen>(ListenThread, false,
                                             pCommonConfig->Ip,pCommonConfig->HttpPort);

  pThreadMgr->CreateComponent<NetworkConnector>(
      ConnectThread, false, (int)NetworkType::HttpConnector, 0);

  app.Run();
  app.Dispose();

  return 0;
}
