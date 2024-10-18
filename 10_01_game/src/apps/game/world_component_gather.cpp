#include "world_component_gather.h"

#include "libserver/message_system_help.h"            // 提供消息系统帮助函数
#include "libplayer/player_collector_component.h"     // 管理玩家收集组件，提供在线玩家统计
#include "libplayer/world_base.h"                     // 世界基础类

// Awake 函数，在对象初始化时调用，添加定时器，用于定期执行同步操作
void WorldComponentGather::Awake()
{
    // 添加定时器，在对象初始化后 10 秒执行 SyncWorldInfoToGather 函数
    // 参数：延迟执行时间、执行间隔时间（0 表示只执行一次）、是否循环执行、定时器ID
    AddTimer(0, 10, false, 0, BindFunP0(this, &WorldComponentGather::SyncWorldInfoToGather));
}

// BackToPool 函数，在对象回收到对象池时调用，执行清理操作
void WorldComponentGather::BackToPool()
{
    // 当前没有需要清理的资源，函数体为空
}

// SyncWorldInfoToGather 函数，将当前世界的信息同步到聚合服务器
void WorldComponentGather::SyncWorldInfoToGather() const
{
    // 构建同步消息协议 proto 对象，填充必要的世界信息
    Proto::WorldProxySyncToGather proto;
    proto.set_world_sn(GetSN());  // 设置世界的序列号
    proto.set_world_proxy_sn(GetSN());  // 设置世界代理的序列号（暂时相同）
    proto.set_world_id(dynamic_cast<IWorld*>(_parent)->GetWorldId());  // 获取世界 ID，并设置

    // 获取当前世界在线玩家数量，并设置到 proto 对象中
    const int online = _parent->GetComponent<PlayerCollectorComponent>()->OnlineSize();
    proto.set_online(online);

    // 通过消息系统将同步消息发送到聚合服务器，消息类型为 MI_WorldProxySyncToGather
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_WorldProxySyncToGather, proto, nullptr);
}
