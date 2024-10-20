#include "global.h" // 引入 Global 类的定义

#if ENGINE_PLATFORM != PLATFORM_WIN32
#include <sys/time.h> // 引入 POSIX 时间相关的定义（仅在非 Windows 平台下）
#include <sys/time.h>
#include <uuid/uuid.h>

#else
#include <objbase.h>
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

// 生成全局唯一标识符 (UUID)
std::string Global::GenerateUUID()
{
#if ENGINE_PLATFORM == PLATFORM_WIN32  // 如果在 Windows 平台上编译
    char buf[64] = { 0 };  // 用于存储生成的 UUID 字符串
    GUID guid;  // Windows 平台的 GUID 结构体，用于生成 UUID
    // 使用 Windows API 生成 GUID，如果成功则将其格式化为字符串
    if (S_OK == ::CoCreateGuid(&guid))
    {
        // 将 GUID 格式化为标准 UUID 字符串形式
        _snprintf_s(buf, sizeof(buf),
            "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
            guid.Data1,
            guid.Data2,
            guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]
        );
    }

    std::string tokenkey = buf;  // 将生成的 UUID 字符串赋值给 tokenkey
#else  // 如果是在非 Windows 平台（如 Linux 或 macOS）上编译
    uuid_t uuid;  // Linux 系统的 uuid_t 结构体，用于生成 UUID
    uuid_generate(uuid);  // 生成 UUID

    char key[36];  // 用于存储生成的 UUID 字符串
    uuid_unparse(uuid, key);  // 将 UUID 转换为字符串格式

    std::string tokenkey = key;  // 将生成的 UUID 字符串赋值给 tokenkey
#endif

    return tokenkey;  // 返回生成的 UUID 字符串
}
