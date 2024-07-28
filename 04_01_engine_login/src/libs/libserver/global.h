#pragma once
#include "common.h"
#include "singleton.h"
#include "time.h"
#include <mutex>

class Global : public Singleton<Global> {
public:
  uint64 GenerateSN();
  int YearDay;
  timeutil::Time TimeTick;

private:
  std::mutex _mtx;
  unsigned int _snTicket{1};
  unsigned int _serverId{0};
};