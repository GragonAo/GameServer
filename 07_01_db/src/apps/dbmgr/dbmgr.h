#pragma once
#include "libserver/thread_mgr.h"
#include "mysql_connector.h"

inline void InitializeComponentDBMgr(ThreadMgr *pThreadMgr) {
  pThreadMgr->CreateComponent<MysqlConnector>();
}