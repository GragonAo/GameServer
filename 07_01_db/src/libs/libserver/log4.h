#pragma once
#include "app_type_mgr.h"
#include "protobuf/proto_id.pb.h"
#include "singleton.h"
#include <log4cplus/logger.h>

class Log4 : public Singleton<Log4> {
public:
  Log4(int appType);
  ~Log4();
  static std::string GetMsgIdName(Proto::MsgId msgId);

protected:
  void DebugInfo(log4cplus::Logger logger) const;

private:
  APP_TYPE _appType{APP_TYPE::APP_None};
};