#include "message_system.h"
#include <utility>
#include "system_manager.h"
#include "packet.h"
#include "entity_system.h"
#include "component.h"
#include "object_pool_packet.h"
#include "component_help.h"

/**
 * @brief 构造函数，初始化消息系统，并与系统管理器关联
 * 
 * @param pMgr 系统管理器指针，用于管理系统生命周期
 */
MessageSystem::MessageSystem(SystemManager* pMgr)
{
    _systemMgr = pMgr;
}

/**
 * @brief Dispose 函数，销毁时释放资源
 */
void MessageSystem::Dispose()
{
    // 资源释放逻辑可以根据需求填充，目前为空
}

/**
 * @brief 将数据包添加到消息队列中
 * 
 * @param pPacket 要添加的 Packet 数据包指针
 * 
 * 该函数通过加锁保护 `_packet_lock`，确保多线程安全。消息包会添加到 `_cachePackets` 的写入缓存区中。
 * 同时，增加消息包的引用计数，以确保其生命周期在处理结束前不会被销毁。
 */
void MessageSystem::AddPacketToList(Packet* pPacket)
{
    // 使用 std::lock_guard 自动管理锁，确保线程安全
    std::lock_guard<std::mutex> guard(_packet_lock);
    
    // 将数据包添加到写入缓存队列
    _cachePackets.GetWriterCache()->emplace_back(pPacket);

    // 增加数据包引用计数，防止提前销毁
    pPacket->AddRef();
}

/**
 * @brief 注册消息回调函数
 * 
 * @param obj 注册回调的实体对象
 * @param msgId 消息ID，表示需要处理的消息类型
 * @param cbfun 回调函数，用于处理指定类型的消息
 * 
 * 该函数将消息ID与对应的回调函数绑定到实体上，确保当收到对应的消息时能够调用相应的回调。
 */
void MessageSystem::RegisterFunction(IEntity* obj, int msgId, MsgCallbackFun cbfun)
{
    // 查找该消息ID是否已经有回调函数注册
    auto iter = _callbacks.find(msgId);
    if (iter == _callbacks.end())
    {
        // 如果没有找到，则为该消息ID创建一个新的回调列表
        _callbacks.insert(std::make_pair(msgId, std::list<IMessageCallBack*>()));
    }

    // 为实体对象添加回调组件，并将回调函数绑定到组件中
    const auto pCallback = obj->AddComponent<MessageCallBack>(std::move(cbfun));
    
    // 将回调组件添加到该消息ID的回调列表中
    _callbacks[msgId].push_back(pCallback);
}

/**
 * @brief 移除组件的消息回调函数
 * 
 * @param obj 要移除回调函数的组件对象
 * 
 * 该函数会遍历所有消息ID对应的回调列表，找到并移除与组件对象对应的回调函数。
 */
void MessageSystem::RemoveFunction(IComponent* obj)
{
    // 遍历所有的回调列表
    for (auto iter1 = _callbacks.begin(); iter1 != _callbacks.end(); ++iter1)
    {
        auto subList = iter1->second;
        // 查找该组件对应的回调
        auto iter2 = std::find_if(subList.begin(), subList.end(), [obj](auto one)
        {
            return one->GetSN() == obj->GetSN();  // 使用组件的序列号 (SN) 进行匹配
        });

        // 如果没有找到，跳过
        if (iter2 == subList.end())
            continue;

        // 找到后，从列表中移除回调
        subList.erase(iter2);
    }
}

/**
 * @brief 每帧更新消息系统，处理接收到的消息包
 * 
 * @param pEntities 实体系统，用于处理消息的实体对象
 * 
 * 该函数会检查消息队列是否有新消息，通过加锁确保消息交换的线程安全。处理完消息后，会调用对应的回调函数。
 */
void MessageSystem::Update(EntitySystem* pEntities)
{
    // 加锁保护消息队列，防止线程冲突
    _packet_lock.lock();
    if (_cachePackets.CanSwap())
    {
        // 如果可以交换，则将写入缓存交换到读取缓存
        _cachePackets.Swap();
    }
    _packet_lock.unlock();

    // 如果读取缓存为空，直接返回
    if (_cachePackets.GetReaderCache()->empty())
        return;

    // 获取读取缓存中的消息包列表
    auto packetLists = _cachePackets.GetReaderCache();
    for (auto iter = packetLists->begin(); iter != packetLists->end(); ++iter)
    {
        auto pPacket = (*iter);
        // 查找该消息包的消息ID是否有对应的回调函数
        const auto finditer = _callbacks.find(pPacket->GetMsgId());
        if (finditer != _callbacks.end())
        {
            auto handleList = finditer->second;
            // 遍历该消息ID的所有回调函数，并依次调用处理
            for (auto pCallBack : handleList)
            {
                pCallBack->ProcessPacket(pPacket);
            }
        }

        // 处理完消息包后，减少引用计数，如果为0则释放
        pPacket->RemoveRef();
    }

    // 处理完后清空读取缓存
    _cachePackets.GetReaderCache()->clear();
}
