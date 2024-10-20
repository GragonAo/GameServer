#pragma once

#include "common.h"

#include <log4cplus/logger.h>       // 引入 log4cplus 日志库的核心类
#include <log4cplus/tstring.h>      // 引入 tstring 用于跨平台字符串处理
#include <log4cplus/fileappender.h> // 用于将日志输出到文件
#include <log4cplus/consoleappender.h> // 用于将日志输出到控制台
#include <log4cplus/loggingmacros.h>   // 用于定义日志宏

// 定义平台相关的颜色代码（主要用于控制台日志的输出颜色）
#if ENGINE_PLATFORM == PLATFORM_WIN32

#define LogColorRed     FOREGROUND_RED
#define LogColorGreen   FOREGROUND_GREEN
#define LogColorYellow  FOREGROUND_RED | FOREGROUND_GREEN
#define LogColorBlue    FOREGROUND_BLUE
#define LogColorPurple  FOREGROUND_BLUE | FOREGROUND_RED
#define LogColorCyan    FOREGROUND_BLUE | FOREGROUND_GREEN
#define LogColorGrey    FOREGROUND_INTENSITY // 灰色
#define LogColorRedEx   FOREGROUND_RED | FOREGROUND_INTENSITY // 高亮红色
#define LogColorGreenEx FOREGROUND_GREEN | FOREGROUND_INTENSITY // 高亮绿色
#define LogColorYellowEx FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY // 高亮黄色
#define LogColorBlueEx  FOREGROUND_BLUE | FOREGROUND_INTENSITY // 高亮蓝色
#define LogColorPurpleEx FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY // 高亮紫色
#define LogColorCyanEx  FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY // 高亮青色
#define LogColorNormal  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE // 默认颜色

#else

// 非 Windows 平台的颜色代码，使用 ANSI 控制码
#define LogColorRed     31
#define LogColorGreen   32
#define LogColorYellow  33
#define LogColorBlue    34
#define LogColorPurple  35
#define LogColorCyan    36
#define LogColorGrey    90  // 深灰色
#define LogColorRedEx   91  // 高亮红色
#define LogColorGreenEx 92  // 高亮绿色
#define LogColorYellowEx 93 // 高亮黄色
#define LogColorBlueEx  94  // 高亮蓝色
#define LogColorPurpleEx 95 // 高亮紫色
#define LogColorCyanEx  96  // 高亮青色

#endif

// 在 Windows 平台上定义设置控制台颜色的函数
#if ENGINE_PLATFORM == PLATFORM_WIN32
void SetColor(int colorEx);
#endif

// 定义不同级别的日志输出宏

// LOG_TRACE 输出追踪信息日志
#define LOG_TRACE(...) { \
    LOG4CPLUS_INFO( log4cplus::Logger::getRoot( ), "["<<  __func__ << "]" <<__VA_ARGS__ ); \
}

// LOG_DEBUG 输出调试级别的日志
#define LOG_DEBUG(...) { \
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot( ), __VA_ARGS__ ); \
}

// LOG_INFO 输出信息级别的日志
#define LOG_INFO(...)  { \
    LOG4CPLUS_INFO( log4cplus::Logger::getRoot( ), __VA_ARGS__ );  \
}

// 定义 Windows 平台上的日志宏，包括颜色支持
#if ENGINE_PLATFORM == PLATFORM_WIN32

// LOG_WARN 输出警告级别的日志，并将输出颜色设置为绿色
#define LOG_WARN(...)  { \
    SetColor( LogColorGreen ); \
    LOG4CPLUS_WARN( log4cplus::Logger::getRoot( ), "[" <<  __func__ << "]" __VA_ARGS__ );\
    SetColor( LogColorNormal ); \
}

// LOG_ERROR 输出错误级别的日志，并将输出颜色设置为红色
#define LOG_ERROR(...)  { \
    SetColor( LogColorRed ); \
    LOG4CPLUS_ERROR( log4cplus::Logger::getRoot( ), "[" <<  __func__ << "]" __VA_ARGS__ );\
    SetColor( LogColorNormal ); \
}

// LOG_COLOR 可以输出自定义颜色的日志
#define LOG_COLOR( colorEx, ...)  { \
    SetColor( colorEx ); \
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot( ), __VA_ARGS__ );\
    SetColor( LogColorNormal ); \
}

#else

// 非 Windows 平台使用 ANSI 颜色控制码进行日志输出
#define LOG_WARN(...)  { \
    LOG4CPLUS_WARN( log4cplus::Logger::getRoot( ),  "\33[0;" << LogColorGreen << "m" <<  __VA_ARGS__ << "\33[0m" );\
}

// 非 Windows 平台的错误日志，使用红色输出
#define LOG_ERROR(...)  { \
    LOG4CPLUS_ERROR( log4cplus::Logger::getRoot( ),  "\33[0;" << LogColorRed << "m" <<  __VA_ARGS__ << "\33[0m" );\
}

// 非 Windows 平台的自定义颜色日志
#define LOG_COLOR( colorEx, msg )  { \
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot( ),  "\33[0;" << colorEx << "m" <<  msg << "\33[0m" );\
}

#endif

// 定义专门用于 GM 日志的宏
#define LOG_GM(...) { \
        log4cplus::Logger gmLogger = log4cplus::Logger::getInstance( LOG4CPLUS_STRING_TO_TSTRING("GM") ); \
        LOG4CPLUS_INFO( gmLogger, __VA_ARGS__ ); \
    }

// 以下是一些未启用的日志宏配置，可以根据需要启用
#if false
#define LOG_HTTP_OPEN 1
#define LOG_HTTP(...)  { LOG_COLOR( LogColorYellowEx, __VA_ARGS__ ) }
#endif

#if false
#define LOG_MSG_OPEN 1
bool IsLogShowMsg(int msgId); // 检查是否显示消息日志的函数
#define LOG_MSG(...)  { LOG_COLOR( LogColorGrey, __VA_ARGS__ ) }
#endif 

#if false
#define LOG_SYSOBJ_OPEN 1
#define LOG_SYSOBJ(...)  { LOG_COLOR( LogColorCyanEx, __VA_ARGS__ ) }
#endif

#if false
#define LOG_REDIS_OPEN 1
#define LOG_REDIS(...)  { LOG_COLOR( LogColorGreenEx, __VA_ARGS__ ) }
#else
#define LOG_REDIS(...)
#endif

#if false
#define LOG_NETWORK_OPEN 1
#define LOG_NETWORK(...)  { LOG_COLOR( LogColorYellowEx, __VA_ARGS__ ) }
#endif
