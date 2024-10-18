#include "log4_help.h" // 包含日志帮助文件
#include "common.h"    // 包含通用定义和宏

#include <list>        // 引入 STL 标准库中的 list 容器

// 如果平台是 Windows，定义控制台颜色设置函数
#if ENGINE_PLATFORM == PLATFORM_WIN32

/**
 * @brief 设置控制台的输出颜色。
 * 
 * @param colorEx 颜色代码，使用 Windows 控制台颜色常量。
 */
void SetColor(const int colorEx)
{
    // 获取标准输出句柄并设置颜色属性
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorEx);
}

#endif // 结束 Windows 平台的条件编译块

// 如果启用了 LOG_MSG_OPEN，定义消息日志显示过滤函数
#if LOG_MSG_OPEN

/**
 * @brief 判断是否显示指定消息 ID 的日志。
 * 
 * @param msgId 消息 ID
 * @return 如果消息应该显示则返回 true，否则返回 false。
 */
bool IsLogShowMsg(const int msgId)
{
    std::list<int> lists; // 定义一个整型列表，用于存储不需要显示的消息 ID

    // 可以在此添加需要过滤的消息 ID，例如：
    // lists.push_back((int)Proto::MI_Ping);
    // lists.push_back((int)Proto::MI_AppInfoSync);
    // lists.push_back((int)Proto::MI_WorldInfoSyncToGather);
    // lists.push_back((int)Proto::MI_WorldProxyInfoSyncToGather);

    // 查找消息 ID 是否在过滤列表中
    const auto iter = std::find(lists.begin(), lists.end(), msgId);
    if (iter != lists.end())
        return false; // 如果找到，表示该消息不应该显示

    return true; // 如果没有找到，表示该消息可以显示
}

#endif // 结束 LOG_MSG_OPEN 条件编译块
