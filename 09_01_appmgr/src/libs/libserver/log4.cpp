#include "log4.h"
#include "protobuf/proto_id.pb.h"
#include "log4_help.h"
#include "res_path.h"
#include "util_string.h"
#include <google/protobuf/descriptor.h>
#include <log4cplus/appender.h>
#include <log4cplus/config.hxx>
#include <log4cplus/configurator.h>
#include <log4cplus/logger.h>
#include <log4cplus/spi/appenderattachable.h>
#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/tstring.h>
#include "component_help.h"

void Log4::Awake(APP_TYPE appType) {
  _appType = static_cast<APP_TYPE>(appType);

  auto pResPath = ComponentHelp::GetResPath();
  const std::string filename = strutil::format(
      "/log4/log4_%s.properties", GetAppName(_appType));
  std::string filePath = pResPath->FindResPath(filename);
  if (filePath.empty()) {
    std::cout << "!!!! log4 properties not fourd! filename." << std::endl;
    return;
  }
  const log4cplus::tstring configFile = LOG4CPLUS_STRING_TO_TSTRING(filePath);

  log4cplus::PropertyConfigurator config(configFile);
  config.configure();

  DebugInfo(log4cplus::Logger::getRoot());
  DebugInfo(log4cplus::Logger::getInstance(LOG4CPLUS_C_STR_TO_TSTRING("GM")));

  LOG_DEBUG("Log4::Initialize is Ok.");
}

void Log4::BackToPool(){}

std::string Log4::GetMsgIdName(Proto::MsgId msgId) {
  const google::protobuf::EnumDescriptor *descriptor =
      Proto::MsgId_descriptor();
  return descriptor->FindValueByNumber(msgId)->name();
}

void Log4::DebugInfo(log4cplus::Logger logger) const {
  log4cplus::SharedAppenderPtrList appenderList = logger.getAllAppenders();
  auto iter = appenderList.end();
  while (iter != appenderList.end()) {
    log4cplus::Appender *pAppender = iter->get();

    std::cout << "[log4] " << LOG4CPLUS_TSTRING_TO_STRING(logger.getName())
              << " appender name:"
              << LOG4CPLUS_TSTRING_TO_STRING(pAppender->getName()) << std::endl;
    ++iter;
  }
}
