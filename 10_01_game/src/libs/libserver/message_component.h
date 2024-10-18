#pragma once

#include "component.h"
#include "message_callback.h"
#include "system.h"

// MessageComponent 继承自 Component 和 IAwakeFromPoolSystem 接口
// 该组件用于处理消息包的回调机制，通过 IMessageCallBackFunction 进行消息处理
class MessageComponent : public Component<MessageComponent>, public IAwakeFromPoolSystem<IMessageCallBackFunction*>
{
public:
    // 析构函数，确保对象销毁时释放相关资源
    ~MessageComponent();

    // 初始化函数（从对象池唤醒时调用）
    // @param pCallback - 消息回调函数指针
    void Awake(IMessageCallBackFunction* pCallback) override;

    // 重置组件状态，并将其返回对象池（避免重复分配内存）
    void BackToPool() override;

    // 判断该组件是否对传入的消息包感兴趣（即是否关注特定消息ID）
    // @param packet - 要检查的消息包
    // @return 如果组件关注该消息ID则返回 true，否则返回 false
    bool IsFollowMsgId(Packet* packet) const;

    // 处理传入的消息包
    // @param packet - 要处理的消息包
    void ProcessPacket(Packet* packet) const;

protected:
    // 存储回调函数的指针，用于消息的具体处理
    IMessageCallBackFunction* _pCallBackFuns{ nullptr };
};
