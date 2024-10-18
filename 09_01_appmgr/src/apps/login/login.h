#pragma once
#include "account.h"
#include "libserver/thread_mgr.h"

inline void InitializeComponentLogin(ThreadMgr *pThreadMgr) {
  pThreadMgr->CreateComponent<Account>();
}