#include "global.h"
#include "common.h"

Global::Global(APP_TYPE appType, int appId){
  _appType = appType;
  _appId = appId;
  std::cout << "app type:" << appType << "id:" << _appId << std::endl;
}

uint64 Global::GenerateSN() {
  std::lock_guard<std::mutex> guard(_mtx);
  uint64 ret = (TimeTick << 32) + (_serverId << 16) + _snTicket;
  _snTicket += 1;
  return ret;
}

APP_TYPE Global::GetCurAppType() const { return _appType; }

int Global::GetCurAppId() const { return _appId; }
