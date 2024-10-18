#include "message_component.h"

// 析构函数
// 在销毁 MessageComponent 对象时，如果回调函数指针 `_pCallBackFuns` 不为空，删除它并将其置为 nullptr。
MessageComponent::~MessageComponent()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;  // 释放回调函数对象内存
        _pCallBackFuns = nullptr;  // 将指针设为 nullptr 以避免悬空指针
    }
}

// 初始化函数 Awake
// 当组件被唤醒时，将传入的回调函数指针 `pCallback` 赋值给 `_pCallBackFuns`。
void MessageComponent::Awake(IMessageCallBackFunction* pCallback)
{
    _pCallBackFuns = pCallback;  // 保存回调函数指针以供后续使用
}

// 返回对象池函数 BackToPool
// 当组件被返回对象池时，清理回调函数对象，释放内存并将指针设为 nullptr。
void MessageComponent::BackToPool()
{
    if (_pCallBackFuns != nullptr)
    {
        delete _pCallBackFuns;  // 释放回调函数对象内存
        _pCallBackFuns = nullptr;  // 防止悬空指针
    }
}

// 检查该组件是否关注指定消息的 ID
// 如果组件有注册回调函数，则检查消息包是否匹配回调函数中的消息 ID。
bool MessageComponent::IsFollowMsgId(Packet* packet) const
{
    if (_pCallBackFuns == nullptr)
        return false;  // 如果没有注册回调函数，返回 false

    return _pCallBackFuns->IsFollowMsgId(packet);  // 调用回调函数对象的方法来判断是否关注消息 ID
}

// 处理消息包
// 如果组件有注册回调函数，则使用该回调函数处理消息包。
void MessageComponent::ProcessPacket(Packet* pPacket) const
{
    if (_pCallBackFuns == nullptr)
        return;  // 如果没有回调函数，直接返回

    _pCallBackFuns->ProcessPacket(pPacket);  // 调用回调函数来处理消息包
}
