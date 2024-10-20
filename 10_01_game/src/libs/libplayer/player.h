#pragma once

#include "libserver/common.h"
#include "libserver/entity.h"
#include "libserver/protobuf/db.pb.h"
#include "libserver/socket_object.h"
#include "libserver/system.h"
#include <sstream>
#include <string>

class Player : public Entity<Player>, 
               public NetworkIdentify, 
               public IAwakeFromPoolSystem<NetworkIdentify*, std::string> {
public:
    // 唤醒玩家实体对象时调用，初始化玩家的网络标识和账户
    void Awake(NetworkIdentify *pIdentify, std::string account) override;

    // 将玩家对象返回到对象池时调用，重置对象状态
    void BackToPool() override;

    // 获取玩家的账户信息
    std::string GetAccount() const;

    // 获取玩家的名字
    std::string GetName() const;

    // 获取玩家的序列号（唯一标识）
    uint64 GetPlayerSN() const;

    // 获取玩家的 Proto 数据对象，便于序列化和传输
    Proto::Player &GetPlayerProto();

    // 从数据流中解析玩家数据，根据玩家序列号从数据流进行反序列化
    void ParseFromStream(uint64 playerSn, std::stringstream *pOpStream);

    // 从 Proto 对象中解析玩家数据，根据玩家序列号从 Proto 对象进行反序列化
    void ParserFromProto(uint64 playerSn, const Proto::Player &proto);

    // 将玩家对象序列化为 Proto 对象，便于传输和存储
    void SerializeToProto(Proto::Player *pProto) const;

protected:
    std::string _account{""};  // 玩家账户信息
    std::string _name{""};     // 玩家名字

    uint64 _playerSn{0};       // 玩家唯一序列号
    Proto::Player _player;     // 玩家相关的 Proto 数据对象
};
