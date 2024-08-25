#include "global.h"
#include "common.h"
#include <sys/time.h>

Global::Global(APP_TYPE appType, int appId){
  _appType = appType;
  _appId = appId;
  std::cout << "app type:" << appType << "id:" << _appId << std::endl;

  UpdateTime();
}

uint64 Global::GenerateSN() {
  std::lock_guard<std::mutex> guard(_mtx);
  uint64 ret = (TimeTick << 32) + (_serverId << 16) + _snTicket;
  _snTicket += 1;
  return ret;
}

APP_TYPE Global::GetCurAppType() const { return _appType; }

int Global::GetCurAppId() const { return _appId; }

void Global::UpdateTime(){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    TimeTick = tv.tv_sec * 1000 + tv.tv_usec * 0.001;
}
