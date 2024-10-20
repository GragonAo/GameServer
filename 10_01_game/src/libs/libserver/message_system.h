#pragma once

#include <map>
#include <list>
#include <functional>
#include <mutex>  // 为了多线程操作引入互斥锁

#include "common.h"
#include "system.h"
#include "cache_swap.h"
#include "socket_object.h"
#include "component_help.h"
#include "packet.h"
#include "message_callback.h"

// 前向声明
class IComponent;
class SystemManager;
class Packet;
class EntitySystem;

/**
 * @class MessageSystem
 * @brief 负责管理消息的系统，继承自 ISystem<MessageSystem> 接口。
 *        该类管理消息的接收、分发、以及消息处理函数的注册与移除。
 */
class MessageSystem : virtual public ISystem<MessageSystem>
{
public:
    // 构造函数，初始化消息系统，并与系统管理器关联
    MessageSystem(SystemManager* pMgr);

    // 销毁函数，释放资源，覆盖自 ISystem 的 Dispose 函数
    void Dispose() override;

    // 更新函数，每帧更新实体系统，覆盖自 ISystem 的 Update 函数
    void Update(EntitySystem* pEntities) override;

    // 将数据包添加到消息队列中，供后续处理
    void AddPacketToList(Packet* pPacket);

    // 注册消息回调函数，绑定实体与消息ID到对应的回调函数
    void RegisterFunction(IEntity* obj, int msgId, MsgCallbackFun cbfun);

    // 移除消息回调函数，删除对应组件的消息处理函数
    void RemoveFunction(IComponent* obj);
    
    // 模板函数，带有过滤器的回调注册，用于根据消息ID和过滤条件注册回调
    template<typename T>
    void RegisterFunctionFilter(IEntity* obj, int msgId, std::function<T * (NetworkIdentify*)> pGetObj, std::function<void(T*, Packet*)> pCallBack);

private:
    // 保护消息队列的互斥锁，防止多线程同时操作消息队列
    std::mutex _packet_lock;

    // 缓存交换区，用于存储 Packet 对象的队列
    CacheSwap<Packet> _cachePackets;

    // 系统管理器指针，负责管理该系统的生命周期
    SystemManager* _systemMgr{ nullptr };

    // 存储消息ID与对应的消息回调函数列表的映射
    std::map<int, std::list<IMessageCallBack*>> _callbacks;
};

/**
 * @brief 带过滤器的回调注册函数模板。
 *        该函数允许注册回调时，提供一个根据 NetworkIdentify 获取对象的过滤函数，以及对应的回调处理函数。
 * 
 * @param obj 实体对象，用于注册消息回调
 * @param msgId 消息ID，标识注册的回调对应的消息类型
 * @param getObj 根据 NetworkIdentify 获取对象的函数
 * @param fun 回调处理函数，用于处理接收到的 Packet 数据包
 */
template <typename T>
void MessageSystem::RegisterFunctionFilter(IEntity* obj, int msgId, std::function<T * (NetworkIdentify*)> getObj, std::function<void(T*, Packet*)> fun)
{
    // 查找是否已有此消息ID的回调注册
    auto iter = _callbacks.find(msgId);
    if (iter == _callbacks.end())
    {
        // 如果没有找到，初始化一个新的消息ID对应的回调列表
        _callbacks.insert(std::make_pair(msgId, std::list<IMessageCallBack*>()));
    }

    // 为实体对象添加一个带有过滤器的回调组件
    auto pCallback = obj->AddComponent<MessageCallBackFilter<T>>();
    // 将传入的过滤函数和回调函数绑定到回调组件
    pCallback->GetFilterObj = std::move(getObj);
    pCallback->HandleFunction = std::move(fun);
    // 将回调组件加入消息ID对应的回调列表中
    _callbacks[msgId].push_back(pCallback);
}
