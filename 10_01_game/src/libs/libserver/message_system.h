#pragma once

#include <map>
#include <mutex>
#include "common.h"
#include "system.h"
#include "cache_swap.h"

class IComponent;
class SystemManager;
class Packet;
class EntitySystem;

class MessageSystem : virtual public ISystem
{
public:
    // 构造函数，接收一个指向系统管理器的指针
    // 该指针用于在消息系统内部与系统管理器交互
    MessageSystem(SystemManager* pMgr);

    // 重写的Dispose函数，用于在销毁该系统时释放资源
    void Dispose() override;

    // 更新函数，每一帧调用，用于处理消息并分发给相关的组件
    void Update(EntitySystem* pEntities) override;

    // 将新的消息包加入消息队列
    // 使用互斥锁确保线程安全
    void AddPacketToList(Packet* pPacket);

private:
    // 静态函数，处理单个消息包并将其分发给相关组件
    // @param pPacket - 待处理的消息包
    // @param lists - 包含所有需要处理该消息包的组件的映射
    static void Process(Packet* pPacket, std::map<uint64, IComponent*>& lists);

private:
    // 多线程锁，用于确保在添加和处理消息时的线程安全
    std::mutex _packet_lock;

    // 使用 CacheSwap 机制存储消息包
    // CacheSwap 是一种双缓冲机制，用于确保在多线程环境下可以安全地交换消息数据
    CacheSwap<Packet> _cachePackets;

    // 系统管理器的指针，用于与系统管理交互
    SystemManager* _systemMgr{ nullptr };
};
