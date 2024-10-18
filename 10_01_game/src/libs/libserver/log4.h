#pragma once

#include "common.h"               // 公共头文件
#include "app_type.h"             // 应用程序类型定义
#include "component.h"            // 组件基础类定义
#include "system.h"               // 系统基础类定义
#include <log4cplus/logger.h>     // log4cplus 日志库

// Log4 类用于日志管理，继承自 Component<Log4> 和 IAwakeSystem<APP_TYPE>
// 它通过日志记录器记录系统运行时的重要信息，并实现了一些组件生命周期的管理方法
class Log4 : public Component<Log4>, public IAwakeSystem<APP_TYPE>
{
public:
    // Awake 方法在系统初始化时调用，传入当前应用程序的类型 appType
    // 该方法用于根据应用程序类型执行日志相关的初始化操作
    void Awake(APP_TYPE appType) override;

    // BackToPool 方法用于将对象返回对象池，清理对象状态
    // 当该组件被回收时调用，确保资源释放和状态重置
    void BackToPool() override;

    // 静态方法，用于根据消息 ID 获取对应的消息名称
    // 该方法将 Proto::MsgId 枚举类型的消息 ID 转换为其对应的字符串名称
    static std::string GetMsgIdName(Proto::MsgId msgId);

protected:
    // 受保护的方法，用于将调试信息记录到日志中
    // logger 是 log4cplus 的日志记录器，实际日志记录在该方法内部实现
    void DebugInfo(log4cplus::Logger logger) const;

private:
    // 私有成员变量，保存当前应用程序的类型
    // APP_TYPE 是一个枚举，_appType 表示当前系统的应用程序类型
    APP_TYPE _appType{ APP_TYPE::APP_None };
};