
#include "dbmgr.h"
#include "libserver/app_type.h"
#include "libserver/component_help.h"
#include "libserver/network_listen.h"
#include "libserver/server_app.h"
#include "libserver/thread_mgr.h"
#include "mysql_table_update.h"
#include <mysql/mysql.h>

int main(int argc, char *argv[]) {
  mysql_library_init(0, nullptr, nullptr);

  const APP_TYPE curAppType = APP_TYPE::APP_DB_MGR;
  ServerApp app(curAppType, argc, argv);
  app.Initialize();

  auto pTableUpdateObj = MysqlTableUpdate::Instance();
  pTableUpdateObj->Check();
  pTableUpdateObj->DestroyInstance();

  auto pThreadMgr = ThreadMgr::GetInstance();
  InitializeComponentDBMgr(pThreadMgr);

  auto pGlobal = Global::GetInstance();
  pThreadMgr->CreateComponent<NetworkListen>(ListenThread, false,
                                             (int)pGlobal->GetCurAppType(),
                                             pGlobal->GetCurAppId());

  app.Run();
  app.Dispose();

  mysql_library_end();
  return 0;
}