#pragma once
#include "account.h"
#include "libserver/thread_mgr.h"
#include "robots/robot_test.h"

inline void InitializeComponentLogin(ThreadMgr *pThreadMgr) {
  pThreadMgr->CreateComponent<RobotTest>();
  pThreadMgr->CreateComponent<Account>();
}