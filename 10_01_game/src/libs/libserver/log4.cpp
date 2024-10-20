#include "common.h"              // 通用定义
#include "log4.h"                // Log4 类的头文件
#include <iostream>              // 标准输入输出
#include <log4cplus/configurator.h>   // log4cplus 配置类
#include <log4cplus/spi/loggingevent.h>  // log4cplus 日志事件
#include <log4cplus/logger.h>     // log4cplus 日志记录器类

#include "util_string.h"         // 字符串处理工具
#include "thread_mgr.h"          // 线程管理器
#include "res_path.h"            // 资源路径管理
#include "app_type.h"            // 应用程序类型定义
#include "log4_help.h"           // log4 辅助函数
#include "component_help.h"      // 组件辅助函数

// Awake 方法在系统启动时被调用，用于初始化日志系统
void Log4::Awake(APP_TYPE appType)
{
    _appType = appType;  // 保存应用程序类型

    // 获取资源路径的组件
    auto pResPath = ComponentHelp::GetResPath();

    // 构建日志配置文件的路径
    const std::string filename = strutil::format("/log4/log4_%s.properties", GetAppName(_appType));
    std::string filePath = pResPath->FindResPath(filename);  // 从资源路径中查找日志配置文件

    // 如果配置文件路径为空，则输出错误信息并返回
    if (filePath.empty())
    {
        std::cout << " !!!!! log4 properties not found! filename:" << filename.c_str() << std::endl;
        return;
    }

    // 将文件路径转换为 log4cplus 字符串类型
    const log4cplus::tstring configFile = LOG4CPLUS_STRING_TO_TSTRING(filePath);

    // 使用配置文件初始化 log4cplus 日志系统
    log4cplus::PropertyConfigurator config(configFile);
    config.configure();

    // 输出调试信息到日志系统
    DebugInfo(log4cplus::Logger::getRoot());  // 输出根日志记录器的调试信息
    DebugInfo(log4cplus::Logger::getInstance(LOG4CPLUS_C_STR_TO_TSTRING("GM")));  // 输出指定名称的日志记录器信息
    LOG_DEBUG("Log4::Initialize is Ok.");  // 输出初始化完成的日志
}

// DebugInfo 方法用于输出日志记录器的调试信息
void Log4::DebugInfo(log4cplus::Logger logger) const
{
    // 获取所有附加器（Appenders），即日志输出目标（如文件或控制台）
    log4cplus::SharedAppenderPtrList appenderList = logger.getAllAppenders();
    auto iter = appenderList.begin();
    
    // 遍历附加器列表并输出每个附加器的名称
    while (iter != appenderList.end())
    {
        log4cplus::Appender* pAppender = iter->get();  // 获取附加器
        std::cout << "[log4] " << LOG4CPLUS_TSTRING_TO_STRING(logger.getName()) 
                  << " appender name:" << LOG4CPLUS_TSTRING_TO_STRING(pAppender->getName()) << std::endl;
        ++iter;
    }
}

// BackToPool 方法用于回收资源，本例中暂不做任何操作
void Log4::BackToPool()
{
}

// GetMsgIdName 方法用于将 Proto::MsgId 枚举类型的消息 ID 转换为其对应的字符串名称
std::string Log4::GetMsgIdName(const Proto::MsgId msgId)
{
    // 获取 Proto::MsgId 枚举的描述符
    const google::protobuf::EnumDescriptor* descriptor = Proto::MsgId_descriptor();
    
    // 根据消息 ID 查找其枚举值并返回对应的名称
    return descriptor->FindValueByNumber(msgId)->name();
}
