#include "log4_help.h"


#if LOG_MSG_OPEN

#include <list>
#include <algorithm> // 用于 std::find

// 判断是否应该显示指定的日志消息
// 参数 msgId: 消息 ID。
// 返回值: 如果消息应该显示，返回 true；否则返回 false。
bool IsLogShowMsg(const int msgId)
{
    std::list<int> lists;

    // 在这里可以添加不需要显示的消息 ID
    // 例如：
    // lists.push_back((int)Proto::MI_Ping);
    // lists.push_back((int)Proto::MI_AppInfoSync);
    // lists.push_back((int)Proto::MI_WorldInfoSyncToGather);
    // lists.push_back((int)Proto::MI_WorldProxyInfoSyncToGather);

    // 检查给定的消息 ID 是否在列表中
    const auto iter = std::find(lists.begin(), lists.end(), msgId);
    if (iter != lists.end())
        return false; // 如果找到，返回 false 表示不显示该消息

    return true; // 如果没找到，返回 true 表示显示该消息
}

#endif
