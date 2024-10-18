#include "message_callback.h"
#include <iterator>

#include "packet.h"
#include "thread_mgr.h"

// 注册消息处理函数
void MessageCallBackFunction::RegisterFunction(int msgId, HandleFunction function)
{
    _callbackHandle[msgId] = function; // 将消息ID与处理函数绑定
}

// 检查包的消息ID是否被关注
bool MessageCallBackFunction::IsFollowMsgId(Packet* packet)
{
    return _callbackHandle.find(packet->GetMsgId()) != _callbackHandle.end(); // 判断消息ID是否存在于处理函数映射中
}

// 处理接收到的包
void MessageCallBackFunction::ProcessPacket(Packet* packet)
{
    // 查找对应消息ID的处理函数
    const auto handleIter = _callbackHandle.find(packet->GetMsgId());
    if (handleIter == _callbackHandle.end())
    {
        std::cout << "packet is not handler. msg id: " << packet->GetMsgId() << std::endl; // 未找到处理函数，输出警告信息
    }
    else
    {
        handleIter->second(packet); // 调用找到的处理函数
    }
}
