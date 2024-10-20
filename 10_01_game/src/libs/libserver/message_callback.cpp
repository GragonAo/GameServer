#include "message_callback.h"

/**
 * @brief 唤醒方法，初始化消息处理回调函数。
 * 
 * 该方法在对象池恢复时调用，接收一个消息处理函数并将其赋值给私有成员 `_handleFunction`。
 * 
 * @param fun 消息处理回调函数，参数为 `Packet*` 类型的网络包指针。
 */
void MessageCallBack::Awake(MsgCallbackFun fun)
{
    _handleFunction = fun;  // 将传入的回调函数保存到类的私有成员变量中
}

/**
 * @brief 回收方法，将对象返回对象池时清理回调函数。
 * 
 * 该方法在对象被回收至对象池时调用，将 `_handleFunction` 置为 `nullptr`，避免使用无效的函数指针。
 */
void MessageCallBack::BackToPool()
{
    _handleFunction = nullptr;  // 清理回调函数，防止下次使用时误调用旧函数
}

/**
 * @brief 处理网络包。
 * 
 * 该方法根据是否启用了日志跟踪组件，记录处理的消息ID和包信息，并调用存储的回调函数 `_handleFunction` 处理具体的 `Packet` 数据。
 * 
 * @param pPacket 指向接收到的网络包 `Packet`。
 */
void MessageCallBack::ProcessPacket(Packet* pPacket)
{
#ifdef LOG_TRACE_COMPONENT_OPEN
    // 使用 ProtoBuf 获取消息ID的描述符
    const google::protobuf::EnumDescriptor* descriptor = Proto::MsgId_descriptor();
    
    // 查找消息ID对应的名字
    const auto name = descriptor->FindValueByNumber(pPacket->GetMsgId())->name();

    // 构建跟踪消息，包含包的序列号和消息ID
    const auto traceMsg = std::string("process. ")
        .append(" sn:").append(std::to_string(pPacket->GetSN()))  // 包的序列号
        .append(" msgId:").append(name);  // 消息ID的名字

    // 将构建的消息通过跟踪组件记录日志
    ComponentHelp::GetTraceComponent()->Trace(TraceType::Packet, pPacket->GetSocketKey().Socket, traceMsg);
#endif

    // 调用存储的回调函数处理网络包
    _handleFunction(pPacket);
}
