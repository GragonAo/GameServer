#pragma once

#include "libserver/common.h"
#include "libserver/component.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/system.h"
#include <sstream>
#include <map>

// PlayerComponentProtoList 类是一个用于管理玩家列表的组件
// 该组件从池中获取，解析和管理玩家的 Proto 数据，并在必要时返回玩家数据的流。
class PlayerComponentProtoList : public Component<PlayerComponentProtoList>,
                                 public IAwakeFromPoolSystem<> {
public:
  // 当组件被初始化时调用，执行需要的初始化操作
  void Awake() override {}

  // 当组件被放回对象池时调用，清理数据并重置状态
  void BackToPool() override;

  // 解析并存储从 Proto::PlayerList 中得到的玩家数据
  // proto: 传入的玩家列表协议数据
  void Parse(Proto::PlayerList &proto);

  // 根据玩家的唯一标识符（sn）获取对应玩家的序列化流
  // sn: 玩家唯一标识符 (uint64)
  // 返回值: 指向 std::stringstream* 对象的指针，包含玩家的 Proto 数据
  std::stringstream *GetProto(uint64 sn);

private:
  // 存储每个玩家的唯一标识符 (sn) 和其对应的 Proto 数据流 (std::stringstream*)
  std::map<uint64, std::stringstream *> _protos;
};
