#include "message_system.h"
#include "system_manager.h"
#include "packet.h"
#include "message_component.h"
#include "entity_system.h"
#include "component.h"
#include "object_pool_packet.h"
#include "component_help.h"

// 构造函数，初始化系统管理器指针
MessageSystem::MessageSystem(SystemManager* pMgr)
{
    _systemMgr = pMgr;
}

// 销毁函数，目前无特定资源需要释放
void MessageSystem::Dispose()
{
    // 在这里可以添加资源清理代码，当前实现为空
}

// 将消息包添加到消息缓存队列中
// 该函数会锁定互斥锁以确保线程安全
// @param pPacket - 要添加的消息包
void MessageSystem::AddPacketToList(Packet* pPacket)
{
    std::lock_guard<std::mutex> guard(_packet_lock); // 线程锁，保证多线程环境下的安全访问
    _cachePackets.GetWriterCache()->emplace_back(pPacket); // 将消息包添加到写缓存中

    // 添加引用计数，防止消息在处理时被提前释放
    pPacket->AddRef();
}

// 每一帧更新函数，负责处理并分发消息包
// @param pEntities - 指向实体系统的指针
void MessageSystem::Update(EntitySystem* pEntities)
{
    // 锁定互斥锁，以确保线程安全地交换消息缓存
    _packet_lock.lock();
    if (_cachePackets.CanSwap())
    {
        _cachePackets.Swap(); // 交换缓存，获取可以处理的消息
    }
    _packet_lock.unlock();

    // 如果没有要处理的消息，直接返回
    if (_cachePackets.GetReaderCache()->size() == 0)
        return;

    // 获取所有与消息系统相关的组件
    auto pCollections = pEntities->GetComponentCollections<MessageComponent>();
    if (pCollections == nullptr)
    {
        // 如果没有找到相关组件，则清空当前的消息队列
        _cachePackets.GetReaderCache()->clear();
        return;
    }

    // 交换组件的缓冲区，确保可以安全地访问它们
    pCollections->Swap();

    // 获取当前的组件列表和消息包列表
    auto lists = pCollections->GetAll();
    auto packetLists = _cachePackets.GetReaderCache();
    
    // 遍历每一个消息包，处理并分发给相关的组件
    for (auto iter = packetLists->begin(); iter != packetLists->end(); ++iter)
    {
        auto pPacket = (*iter);
        Process(pPacket, lists); // 处理并分发消息包

        // 消息包处理完后，减少引用计数
        pPacket->RemoveRef();
    }

    // 清空消息包队列
    _cachePackets.GetReaderCache()->clear();
}

// 处理函数，将消息包分发给所有与之相关的组件
// @param pPacket - 要处理的消息包
// @param lists - 需要处理该消息包的组件列表
void MessageSystem::Process(Packet* pPacket, std::map<uint64, IComponent*>& lists)
{
    // 遍历所有的组件
    for (auto iter = lists.begin(); iter != lists.end(); ++iter)
    {
        // 将 IComponent 强制转换为 MessageComponent
        MessageComponent* pMsgComponent = static_cast<MessageComponent*>(iter->second);

        // 检查组件是否需要处理该消息包
        if (pMsgComponent->IsFollowMsgId(pPacket))
        {
#ifdef LOG_TRACE_COMPONENT_OPEN
            // 如果启用了跟踪日志，则记录消息包的处理信息
            const google::protobuf::EnumDescriptor* descriptor = Proto::MsgId_descriptor();
            const auto name = descriptor->FindValueByNumber(pPacket->GetMsgId())->name();

            auto pParent = pMsgComponent->GetParent();
            const auto traceMsg = std::string("process. ")
                .append(" sn:").append(std::to_string(pPacket->GetSN()))
                .append(" msgId:").append(name)
                .append(" process by:").append(pParent->GetTypeName());

            // 记录消息处理的跟踪日志
            ComponentHelp::GetTraceComponent()->Trace(TraceType::Packet, pPacket->GetSocketKey().Socket, traceMsg);
#endif
            // 调用组件的消息处理函数，处理该消息包
            pMsgComponent->ProcessPacket(pPacket);
        }
    }
}
