#pragma once

#include "console_app_component.h"
#include "console_cmd_app.h"
#include "libserver/console.h"
#include "libserver/thread_mgr.h"
#include "login_sync_component.h"

inline void InitializeComponentAppMgr(ThreadMgr *pThreadMgr) {
  pThreadMgr->CreateComponent<LoginSyncComponent>();

  auto pConsole = pThreadMgr->GetEntitySystem()->GetComponent<Console>();
  pConsole->Register<ConsoleCmdApp>("app");

  pThreadMgr->CreateComponent<ConsoleAppComponent>(LogicThread, true);
}