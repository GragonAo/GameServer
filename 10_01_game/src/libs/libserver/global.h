#pragma once // 确保头文件只被包含一次

#include "common.h"      // 引入常用定义和类型
#include "util_time.h"   // 引入时间工具相关的定义
#include "app_type.h"    // 引入应用类型的定义
#include "singleton.h"   // 引入单例模板类的定义

#include <mutex>         // 引入互斥锁支持

// Global 类，使用单例模式
class Global : public Singleton<Global>
{
public:
    // 构造函数，初始化应用类型和 ID
    Global(APP_TYPE appType, int appId);

    // 更新当前时间
    void UpdateTime();

    // 生成唯一的序列号 (SN)
    uint64 GenerateSN(); // SN = 64位，包含时间戳 + 服务器ID + ticket

    // 获取当前应用类型
    APP_TYPE GetCurAppType() const;

    // 获取当前应用 ID
    int GetCurAppId() const;

    // 当前年中的天数
    int YearDay;

    // 当前时间的时间戳
    timeutil::Time TimeTick;

    // 停止标志
    bool IsStop{ false };

private:
    // 互斥锁，用于保证线程安全
    std::mutex _mtx;

    // SN ticket，用于生成唯一的序列号
    unsigned short _snTicket{ 1 };

    // 服务器 ID
    unsigned int _serverId{ 0 };

    // 当前应用类型
    APP_TYPE _appType;

    // 当前应用 ID
    int _appId{ 0 };
};
