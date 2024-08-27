#pragma once
#include "app_type.h"
#include "singleton.h"
#include "util_time.h"
#include <mutex>

class Global : public Singleton<Global> {
public:
  Global(APP_TYPE appType, int appId);
  void UpdateTime();
  
  uint64 GenerateSN();

  APP_TYPE GetCurAppType() const;
  int GetCurAppId() const;

  int YearDay;
  timeutil::Time TimeTick;
  bool IsStop{false};

private:
  std::mutex _mtx;
  unsigned int _snTicket{1};
  unsigned int _serverId{0};

  APP_TYPE _appType;
  int _appId{0};
};