#pragma once

#include "libserver/system.h"
#include "libserver/entity.h"

class Packet;  // 前置声明 Packet 类，用于处理网络数据包

// WorldProxyInfo 结构体，保存每个 WorldProxy 的基本信息
struct WorldProxyInfo
{
    uint64 WorldProxySn; // 世界代理的唯一序列号
    int WorldId;         // 世界 ID
    int Online;          // 在线人数
};

// WorldProxyGather 类，管理和同步多个 WorldProxy 的数据
class WorldProxyGather : public Entity<WorldProxyGather>, public IAwakeSystem<>
{
public:
    // Awake 函数，在实例创建时调用，用于初始化逻辑
    void Awake() override;

    // BackToPool 函数，在对象回收到对象池时调用，用于清理资源
    void BackToPool() override;

private:
    // 同步当前游戏信息到所有 WorldProxy
    void SyncGameInfo();

    // 处理来自 WorldProxy 的同步数据包
    void HandleWorldProxySyncToGather(Packet* pPacket);

    // 处理与 WorldProxy 相关的控制命令
    void HandleCmdWorldProxy(Packet* pPacket);

private:
    // 保存每个 WorldProxy 的信息，键为 WorldProxy 的序列号，值为对应的 WorldProxyInfo
    std::map<uint64, WorldProxyInfo> _maps;
};
