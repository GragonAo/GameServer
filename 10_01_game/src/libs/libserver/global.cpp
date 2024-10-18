#include "global.h" // 引入 Global 类的定义

#if ENGINE_PLATFORM != PLATFORM_WIN32
#include <sys/time.h> // 引入 POSIX 时间相关的定义（仅在非 Windows 平台下）
#endif

// Global 构造函数，接受应用类型和 ID
Global::Global(APP_TYPE appType, int appId)
{
    _appType = appType; // 设置当前应用类型
    _appId = appId;     // 设置当前应用 ID
    std::cout << "app type:" << GetAppName(appType) << " id:" << _appId << std::endl; // 输出应用信息

    UpdateTime(); // 更新当前时间
}

// 生成唯一的序列号 (SN)
uint64 Global::GenerateSN()
{
    std::lock_guard<std::mutex> guard(_mtx); // 使用锁保护共享资源，确保线程安全
    // (40位时间戳, 8位服务器ID, 16位SN ticket)
    const uint64 ret = (TimeTick >> 8 << 24) + (_serverId << 16) + _snTicket; // 组合 SN
    _snTicket += 1; // 增加 SN ticket
    return ret; // 返回生成的 SN
}

// 获取当前应用类型
APP_TYPE Global::GetCurAppType() const
{
    return _appType; // 返回当前应用类型
}

// 获取当前应用 ID
int Global::GetCurAppId() const
{
    return _appId; // 返回当前应用 ID
}

// 更新当前时间
void Global::UpdateTime()
{
#if ENGINE_PLATFORM != PLATFORM_WIN32
    struct timeval tv; // 定义 timeval 结构体，用于获取时间
    gettimeofday(&tv, nullptr); // 获取当前时间
    TimeTick = tv.tv_sec * 1000 + tv.tv_usec * 0.001; // 将秒和微秒转换为毫秒并存储
#else
    // 使用 C++11 的时间库获取当前时间
    auto timeValue = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    TimeTick = timeValue.time_since_epoch().count(); // 获取从纪元（1970年1月1日）以来的毫秒数
#endif

    //#if ENGINE_PLATFORM != PLATFORM_WIN32
    //    auto tt = std::chrono::system_clock::to_time_t(timeValue);
    //    struct tm* ptm = localtime(&tt);
    //    YearDay = ptm->tm_yday; // 获取一年中的天数
    //#else
    //    auto tt = std::chrono::system_clock::to_time_t(timeValue);
    //    struct tm tm;
    //    localtime_s(&tm, &tt);
    //    YearDay = tm.tm_yday; // 获取一年中的天数
    //#endif
}
