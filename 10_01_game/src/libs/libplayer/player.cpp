#include "player.h"
#include "libserver/protobuf/db.pb.h"
#include "libserver/socket_object.h"
#include "player_component.h"
#include <sstream>

// 唤醒玩家对象时调用，初始化玩家的账户信息、序列号和网络标识
void Player::Awake(NetworkIdentify *pIdentify, std::string account) {
  // 初始化玩家账户和序列号
  _account = account;
  _playerSn = 0;  // 玩家序列号默认设置为 0
  _player.Clear();  // 清空玩家的 Proto 对象

  // 如果网络标识不为空，设置玩家的 socket 连接 key
  if (pIdentify != nullptr)
    _socketKey = pIdentify->GetSocketKey();

  // 设置对象的唯一标识（以账户为依据）
  _objKey = {ObjectKeyType::Account, ObjectKeyValue{0, _account}};
}

// 返回玩家对象到对象池时调用，重置所有成员变量
void Player::BackToPool() {
  // 清空玩家账户、名字、序列号和 Proto 对象
  _account = "";
  _name = "";
  _playerSn = 0;
  _player.Clear();

  // 清空网络和对象标识
  _socketKey.Clean();
  _objKey.Clean();
}

// 获取玩家的账户信息
std::string Player::GetAccount() const { 
  return _account; 
}

// 获取玩家的名字
std::string Player::GetName() const { 
  return _name; 
}

// 获取玩家的序列号
uint64 Player::GetPlayerSN() const { 
  return _playerSn; 
}

// 获取玩家的 Proto 数据对象，用于序列化和反序列化
Proto::Player &Player::GetPlayerProto() { 
  return _player; 
}

// 从数据流中解析玩家数据，根据传入的序列号和输入流进行反序列化
void Player::ParseFromStream(const uint64 playerSn,
                             std::stringstream *pOpStream) {
  _playerSn = playerSn;  // 设置玩家序列号
  _player.ParseFromIstream(pOpStream);  // 从输入流中解析 Proto 数据
}

// 从 Proto 对象中解析玩家数据，并更新玩家的相关信息
void Player::ParserFromProto(const uint64 playerSn,
                             const Proto::Player &proto) {
  _playerSn = playerSn;  // 设置玩家序列号
  _player.CopyFrom(proto);  // 从传入的 Proto 对象中复制玩家数据
  _name = _player.name();  // 更新玩家名字

  // 遍历玩家的所有组件，调用对应组件的反序列化函数
  for (auto pair : _components) {
    auto pPlayerComponet = dynamic_cast<PlayerComponent *>(pair.second);
    if (pPlayerComponet == nullptr)
      continue;

    // 调用组件的从 Proto 对象解析数据的函数
    pPlayerComponet->ParserFromProto(proto);
  }
}

// 将玩家对象序列化为 Proto 对象，用于数据传输或存储
void Player::SerializeToProto(Proto::Player *pProto) const {
  pProto->CopyFrom(_player);  // 将玩家的当前数据复制到 Proto 对象中

  // 遍历玩家的所有组件，调用对应组件的序列化函数
  for (auto pair : _components) {
    auto pPlayerComponent = dynamic_cast<PlayerComponent *>(pair.second);
    if (pPlayerComponent == nullptr)
      continue;

    // 调用组件的序列化函数，将组件的数据添加到 Proto 对象
    pPlayerComponent->SerializeToProto(pProto);
  }
}
