#pragma once

#include "libplayer/world_base.h"
#include "libserver/system.h"
#include "libserver/entity.h"

// 预声明 Packet 和 Player 类，避免头文件的循环依赖
class Packet;
class Player;

// Lobby 类，继承自 Entity<Lobby>、IWorld 和 IAwakeSystem<>。
// - Entity<Lobby>：表示该类是一个实体，模板参数为自身。
// - IWorld：接口，表示此类与世界管理相关。
// - IAwakeSystem<>：系统接口，表示此类具有唤醒和回收到对象池的能力。
class Lobby : public Entity<Lobby>, public IWorld, public IAwakeSystem<>
{
public:
    // Override 自 IAwakeSystem<> 的虚函数 Awake，用于初始化逻辑
	void Awake() override;

    // Override 自 IAwakeSystem<> 的虚函数 BackToPool，用于对象回收逻辑
	void BackToPool() override;

private:
    // 处理网络断开连接的函数，参数为接收到的 Packet 数据包指针
    void HandleNetworkDisconnect(Packet* pPacket);

    // 通过 Token 进行登录的处理函数，参数为接收到的 Packet 数据包指针
    void HandleLoginByToken(Packet* pPacket);

private:
    // 等待分配到世界的玩家集合。
    // 键为 int 类型（通常是表示世界的 ID 或者类似标识），值为一组 uint64 类型（玩家的唯一 ID）。
	std::map<int, std::set<uint64>> _waitingForWorld;
};
