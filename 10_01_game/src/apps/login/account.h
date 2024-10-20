#pragma once

#include "libplayer/player.h"
#include "libserver/entity.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/socket_object.h"
#include "libserver/sync_component.h"
#include "libserver/system.h"

#include <json/reader.h>
#include <string>

class Account : public Entity<Account>,public IAwakeSystem<>,public SyncComponent {
public:
    // 当对象从对象池唤醒时调用，用于初始化
    void Awake() override;

    // 当对象返回对象池时调用，用于清理和重置状态
    virtual void BackToPool() override;

private:
    // 同步应用程序信息到 AppMgr
    void SyncAppInfoToAppMgr();
    // 处理从 AppMgr 同步来的应用程序信息列表
    void HandleAppInfoListSync(Packet *pPacket);

    // 处理网络连接事件
    void HandleNetworkConnected(Packet *pPacket);
    // 处理网络断开连接事件
    void HandleNetworkDisconnect(Packet *pPacket);
    // 在网络断开时处理与该账户关联的网络标识
    void SocketDisconnect(std::string account, NetworkIdentify *pIdentify);

    // 处理来自外部 HTTP 请求的响应
    void HandleHttpOuterResponse(Packet *pPacket);

    // 处理账户检查请求
    void HandleAccountCheck(Packet *pPacket);
    // 处理查询账户是否在线的响应
    void HandleAccountQueryOnlineToRedisRs(Packet* pPacket);
    // 处理查询玩家列表的响应
    void HandleQueryPlayerListRs(Packet *pPacket);

    // 处理创建玩家请求
    void HandleCreatePlayer(Packet *pPacket);
    // 处理创建玩家的响应
    void HandleCreatePlayerRs(Packet *pPacket);

    // 处理选择玩家请求
    void HandleSelectPlayer(Packet *pPacket);
    //处理请求Token
    void RequestToken(Player* pPlayer) const;
    //处理Token到Redis的响应
    void HandleTokenToRedisRs(Packet* pPacket);


private:
    // 处理账户检查的具体逻辑，返回检查结果代码
    Proto::AccountCheckReturnCode ProcessMsg(Json::Value value) const;

private:
    std::string _method{""};    // 存储 HTTP 请求方法
    std::string _httpIp{""};    // 存储 HTTP 请求的 IP 地址
    int _httpPort{0};           // 存储 HTTP 请求的端口
    std::string _path{""}; // 存储 HTTP 路径的端口
};
