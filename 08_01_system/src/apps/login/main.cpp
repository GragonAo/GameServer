#include "libserver/app_type.h"
#include "libserver/component_help.h"
#include "libserver/network_connector.h"
#include "libserver/network_listen.h"
#include "libserver/server_app.h"
#include "libserver/thread_mgr.h"
#include "libserver/thread_type.h"
#include "libserver/yaml.h"
#include "login.h"
int main(int argc, char *argv[]) {
  const APP_TYPE curAppType = APP_TYPE::APP_LOGIN;
  ServerApp app(curAppType, argc, argv);
  app.Initialize();

  auto pThreadMgr = ThreadMgr::GetInstance();
  InitializeComponentLogin(pThreadMgr);

  auto pYaml = ComponentHelp::GetYaml();
  auto pCommonConfig = pYaml->GetIPEndPoint(curAppType);

  pThreadMgr->CreateThread(ListenThread, 1);
  pThreadMgr->CreateComponent<NetworkListen>(ListenThread,pCommonConfig->Ip,
                                             pCommonConfig->Port);

  pThreadMgr->CreateThread(ConnectThread, 1);
  pThreadMgr->CreateComponent<NetworkConnector>(ConnectThread,(int)APP_TYPE::APP_DB_MGR, 0);

  app.Run();
  app.Dispose();
  return 0;
}
