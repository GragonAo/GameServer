#pragma once

#include "player.h"
#include "player_component_proto_list.h"
#include "libserver/common.h"
#include "libserver/socket_object.h"
#include "libserver/system.h"

// PlayerCollectorComponent 类用于管理在线玩家的集合，包括添加、删除和查找玩家的功能
class PlayerCollectorComponent : public Component<PlayerCollectorComponent>,
                                 public IAwakeFromPoolSystem<> {
public:
  // 重写 Awake 方法，在组件被初始化时调用
  void Awake() override;

  // 重写 BackToPool 方法，在组件返回对象池时调用，执行清理操作
  void BackToPool() override;

  // 添加新玩家，关联其网络标识符和账户
  Player *AddPlayer(NetworkIdentify *pIdentify, std::string account);

  // 根据 socket 关闭连接时移除玩家
  void RemovePlayerBySocket(SOCKET socket);

  // 根据玩家唯一标识符 (sn) 移除玩家
  void RemovePlayerBySn(uint64 playersn);

  // 根据 socket 获取对应的玩家对象
  Player *GetPlayerBySocket(SOCKET socket);

  // 根据账户名获取对应的玩家对象
  Player *GetPlayerByAccount(std::string account);

  // 根据玩家唯一标识符 (sn) 获取对应的玩家对象
  Player *GetPlayerBySn(uint64 playerSn);

  // 获取当前在线玩家的数量
  int OnlineSize() const;

  // 获取所有在线玩家的映射
  std::map<SOCKET, Player *> &GetAll();

private:
  // 存储玩家与其对应 socket 的映射
  std::map<SOCKET, Player *> _players;

  // 存储账户与其对应 socket 的映射
  std::map<std::string, SOCKET> _accounts;
};
