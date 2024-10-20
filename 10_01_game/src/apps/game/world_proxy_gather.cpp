#include "world_proxy_gather.h"

#include "libserver/message_system_help.h"   // 提供消息系统帮助函数
#include "libserver/message_system.h"
#include "libserver/thread_mgr.h"            // 线程管理类
#include "libserver/global.h"                // 全局管理器，提供应用程序级别的信息

#include <numeric>  // 用于标准库中的 accumulate 函数

// Awake 函数，在对象创建时调用，用于初始化逻辑
void WorldProxyGather::Awake()
{
    // 添加定时器，每 10 秒调用一次 SyncGameInfo 函数进行游戏信息同步
    AddTimer(0, 10, true, 2, BindFunP0(this, &WorldProxyGather::SyncGameInfo));

    // 注册消息回调
    auto pMgrSystem = GetSystemManager()->GetMessageSystem();

    // 注册处理世界代理同步消息的回调
    pMgrSystem->RegisterFunction(this,Proto::MsgId::MI_WorldProxySyncToGather, BindFunP1(this, &WorldProxyGather::HandleWorldProxySyncToGather));

    // 注册处理世界代理命令消息的回调
    pMgrSystem->RegisterFunction(this,Proto::MsgId::MI_CmdWorldProxy, BindFunP1(this, &WorldProxyGather::HandleCmdWorldProxy));
}

// BackToPool 函数，在对象回收到对象池时调用，清理资源
void WorldProxyGather::BackToPool()
{
    _maps.clear();  // 清空存储的 world proxy 信息
}

// SyncGameInfo 函数，负责定期同步游戏信息到 APP_APPMGR
void WorldProxyGather::SyncGameInfo()
{
    Proto::AppInfoSync proto;

    // 计算所有 world proxy 的在线人数之和
    const int online = std::accumulate(_maps.begin(), _maps.end(), 0, [](int value, auto pair)
        {
            return value + pair.second.Online;  // 累加每个 world proxy 的在线人数
        });

    // 设置应用程序的 ID、类型和在线人数
    proto.set_app_id(Global::GetInstance()->GetCurAppId());
    proto.set_app_type((int)Global::GetInstance()->GetCurAppType());
    proto.set_online(online);

    // 发送同步消息到 APP_APPMGR
    MessageSystemHelp::SendPacket(Proto::MsgId::MI_AppInfoSync, proto, APP_APPMGR);
}

// HandleCmdWorldProxy 函数，处理命令消息，用于显示当前 world proxy 的状态信息
void WorldProxyGather::HandleCmdWorldProxy(Packet* pPacket)
{
    LOG_DEBUG("------------------------------------");
    LOG_DEBUG("**** world proxy gather ****");

    // 遍历 _maps 中的所有 world proxy，打印其序列号和在线人数
    for (auto one : _maps)
    {
        LOG_DEBUG("sn:" << one.first
            << " proxy sn:" << one.second.WorldProxySn
            << " online:" << one.second.Online);
    }
}

// HandleWorldProxySyncToGather 函数，处理 world proxy 同步到 gather 的数据包
void WorldProxyGather::HandleWorldProxySyncToGather(Packet* pPacket)
{
    // 解析收到的消息数据包
    auto proto = pPacket->ParseToProto<Proto::WorldProxySyncToGather>();
    const uint64 worldSn = proto.world_sn();  // 获取 world 的序列号
    const int worldId = proto.world_id();     // 获取 world 的 ID
    const int online = proto.online();        // 获取 world 的在线人数
    const auto proxySn = proto.world_proxy_sn();  // 获取 world proxy 的序列号

    // 查找对应的 world proxy 信息，如果没有则创建新的条目
    auto iter = _maps.find(worldSn);
    if (iter == _maps.end())
    {
        _maps[worldSn] = WorldProxyInfo();  // 新建 world proxy 信息
        _maps[worldSn].WorldId = worldId;
        _maps[worldSn].WorldProxySn = proxySn;
    }

    // 检查同步的 proxy 序列号是否一致，不一致则输出警告日志
    if (proxySn != _maps[worldSn].WorldProxySn)
    {
        LOG_WARN("error. sync proxy info. proxySn != _maps[worldSn].WorldProxySn");
    }

    // 更新 world proxy 的在线人数
    _maps[worldSn].Online = online;
}
