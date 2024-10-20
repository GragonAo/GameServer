#pragma once
#include "packet.h"

/**
 * @brief 消息回调接口类，用于处理接收到的网络包。
 * 
 * 该类继承自 `Component<IMessageCallBack>`，表示它是一个组件，并且提供了纯虚函数 `ProcessPacket`，需要派生类实现具体的消息处理逻辑。
 */
class IMessageCallBack : public Component<IMessageCallBack>
{
public:
    virtual ~IMessageCallBack() = default;  // 虚析构函数，确保子类的正确析构

    /**
     * @brief 处理网络包的纯虚函数。
     * 
     * 该函数由派生类实现，用于处理接收到的 `Packet` 数据包。
     * @param packet 指向接收到的网络包指针。
     */
    virtual void ProcessPacket(Packet* packet) = 0;
};

// 定义了消息回调函数类型，参数为接收到的网络包指针
using MsgCallbackFun = std::function<void(Packet*)>;

/**
 * @brief 消息回调类，继承自 `IMessageCallBack` 和 `IAwakeFromPoolSystem<MsgCallbackFun>`。
 * 
 * 该类通过 `Awake` 方法初始化，并重写 `ProcessPacket` 函数来处理消息。
 */
class MessageCallBack : public IMessageCallBack, public IAwakeFromPoolSystem<MsgCallbackFun>
{
public:
    /**
     * @brief 唤醒函数，用于从对象池中恢复时初始化回调函数。
     * 
     * @param fun 消息处理函数，参数为接收到的网络包指针。
     */
    void Awake(MsgCallbackFun fun) override;

    /**
     * @brief 将回调对象返回对象池时的清理操作。
     * 
     * 将 `_handleFunction` 设置为 `nullptr`，确保回调函数被正确清理。
     */
    void BackToPool() override;

    /**
     * @brief 处理网络包的具体实现。
     * 
     * 该方法通过 `_handleFunction` 处理接收到的网络包。
     * @param pPacket 接收到的网络包指针。
     */
    virtual void ProcessPacket(Packet* pPacket) override;

private:
    MsgCallbackFun _handleFunction;  // 保存回调函数，用于处理接收到的网络包
};

/**
 * @brief 泛型消息过滤回调类，继承自 `IMessageCallBack` 和 `IAwakeFromPoolSystem<>`。
 * 
 * 该类允许通过过滤函数 `GetFilterObj` 获取对象，并通过 `HandleFunction` 处理消息。
 * 
 * @tparam T 过滤对象的类型。
 */
template<class T>
class MessageCallBackFilter : public IMessageCallBack, public IAwakeFromPoolSystem<>
{
public:
    /**
     * @brief 唤醒函数（此处为空实现）。
     */
    void Awake() override {}

    /**
     * @brief 将回调对象返回对象池时的清理操作。
     * 
     * 将 `HandleFunction` 和 `GetFilterObj` 清空，确保回调函数被正确清理。
     */
    void BackToPool() override
    {
        HandleFunction = nullptr;
        GetFilterObj = nullptr;
    }

    /**
     * @brief 处理网络包的具体实现。
     * 
     * 通过 `GetFilterObj` 获取指定的过滤对象，并通过 `HandleFunction` 对其进行处理。如果找不到对象，直接返回。
     * 
     * @param pPacket 接收到的网络包指针。
     */
    virtual void ProcessPacket(Packet* pPacket) override
    {
        auto pObj = GetFilterObj(pPacket);  // 使用过滤函数获取对象
        if (pObj == nullptr)
            return;  // 如果找不到对象，则不处理

#ifdef LOG_TRACE_COMPONENT_OPEN
        // 使用 ProtoBuf 获取消息 ID 的描述信息
        const google::protobuf::EnumDescriptor* descriptor = Proto::MsgId_descriptor();
        const auto name = descriptor->FindValueByNumber(pPacket->GetMsgId())->name();

        // 构建日志信息并记录
        const auto traceMsg = std::string("process. ")
            .append(" sn:").append(std::to_string(pPacket->GetSN()))
            .append(" msgId:").append(name);
        ComponentHelp::GetTraceComponent()->Trace(TraceType::Packet, pPacket->GetSocketKey().Socket, traceMsg);
#endif

        // 调用回调函数处理过滤后的对象
        HandleFunction(pObj, pPacket);
    }

    // 回调处理函数，用于处理过滤后的对象
    std::function<void(T*, Packet*)> HandleFunction{ nullptr };

    // 过滤函数，根据网络标识获取需要处理的对象
    std::function<T * (NetworkIdentify*)> GetFilterObj{ nullptr };
};
