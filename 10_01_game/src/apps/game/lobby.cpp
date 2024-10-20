#include "lobby.h"

#include "player_component_onlinegame.h"
#include "player_component_token.h"
#include "libserver/entity_system.h"
#include "libserver/message_system.h"
#include "libserver/message_system_help.h"
#include "libplayer/player_collector_component.h"
#include "libplayer/player_component_proto_list.h"
#include "world_component_gather.h"

// Lobby 类的 Awake 函数，在对象初始化时被调用，负责添加必要的组件和注册消息处理函数
void Lobby::Awake()
{
    // 添加负责收集玩家信息的组件 PlayerCollectorComponent
    AddComponent<PlayerCollectorComponent>();

    // 添加世界组件聚合类，用于管理世界相关的数据
    AddComponent<WorldComponentGather>();

    // 创建消息回调函数对象，用于处理网络消息
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();

    // 注册处理网络断开事件的回调函数，关联消息 ID 为 MI_NetworkDisconnect
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_NetworkDisconnect, BindFunP1(this, &Lobby::HandleNetworkDisconnect));

    // 注册处理通过 Token 登录的回调函数，关联消息 ID 为 C2G_LoginByToken
    pMsgSystem->RegisterFunction(this,Proto::MsgId::C2G_LoginByToken, BindFunP1(this, &Lobby::HandleLoginByToken));

    //注册处理Token到Redis的响应
    pMsgSystem->RegisterFunction(this, Proto::MsgId::MI_GameTokenToRedisRs, BindFunP1(this, &Lobby::HandleGameTokenToRedisRs));
}

// BackToPool 函数在对象回收到池中时调用，负责清空等待分配世界的玩家列表
void Lobby::BackToPool()
{
    // 清空等待分配到游戏世界的玩家集合
    _waitingForWorld.clear();
}

// 处理网络断开事件，当玩家断开连接时调用该函数
void Lobby::HandleNetworkDisconnect(Packet* pPacket)
{
    // 从玩家收集器中移除断开连接的玩家
    GetComponent<PlayerCollectorComponent>()->RemovePlayerBySocket(pPacket->GetSocketKey().Socket);
}

// 处理通过 Token 登录事件
void Lobby::HandleLoginByToken(Packet* pPacket)
{
    // 获取玩家收集器组件，用于管理玩家
    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    // 解析登录请求的协议数据
    auto proto = pPacket->ParseToProto<Proto::LoginByToken>();

    // 根据解析到的账号信息添加玩家到收集器中
    auto pPlayer = pPlayerCollector->AddPlayer(pPacket, proto.account());
    if (pPlayer == nullptr)
    {
        // 如果无法添加玩家，发送断开请求的消息
        MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect, pPacket);
        return;
    }
    //添加 用于处理玩家的 Token、玩家在游戏中的在线状态的组件
    pPlayer->AddComponent<PlayerComponentToken>(proto.token());
    pPlayer->AddComponent<PlayerComponentOnlineInGame>(pPlayer->GetAccount(), 1);

    Proto::GameTokenToRedis protoToken;
    protoToken.set_account(pPlayer->GetAccount().c_str());
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_GameTokenToRedis, protoToken, nullptr);
}

void Lobby::HandleGameTokenToRedisRs(Packet* pPacket)
{
    // 解析从 Redis 返回的游戏 Token 响应消息
    auto protoRs = pPacket->ParseToProto<Proto::GameTokenToRedisRs>();

    // 根据账号获取对应的玩家对象
    auto pPlayer = GetComponent<PlayerCollectorComponent>()->GetPlayerByAccount(protoRs.account());
    if (pPlayer == nullptr)
    {
        // 如果找不到玩家对象，记录错误日志并返回
        LOG_ERROR("HandleGameRequestTokenToRedisRs. pPlayer == nullptr. account:" << protoRs.account().c_str());
        return;
    }

    // 准备返回的登录结果消息，默认 Token 错误
    Proto::LoginByTokenRs protoLoginGameRs;
    protoLoginGameRs.set_return_code(Proto::LoginByTokenRs::LGRC_TOKEN_WRONG);

    // 获取玩家的 Token 组件，校验从 Redis 返回的 Token 是否有效
    const auto pTokenComponent = pPlayer->GetComponent<PlayerComponentToken>();
    if (pTokenComponent->IsTokenValid(protoRs.token_info().token()))
    {
        // 如果 Token 有效，则设置返回码为成功
        protoLoginGameRs.set_return_code(Proto::LoginByTokenRs::LGRC_OK);
    }

    // 发送登录 Token 校验结果给客户端
    MessageSystemHelp::SendPacket(Proto::MsgId::C2G_LoginByTokenRs, pPacket, protoLoginGameRs);

    // 如果 Token 校验失败，则不继续后续操作
    if (protoLoginGameRs.return_code() != Proto::LoginByTokenRs::LGRC_OK)
        return;

    // 如果登录成功，记录玩家进入游戏的日志
    LOG_DEBUG("enter game. account:" << pPlayer->GetAccount().c_str() << " token:" << protoRs.token_info().token().c_str());

    // 查询玩家的具体信息
    // Proto::QueryPlayer protoQuery;
    // protoQuery.set_player_sn(protoRs.token_info().player_sn());
    // MessageSystemHelp::SendPacket(Proto::MsgId::G2DB_QueryPlayer, protoQuery, APP_DB_MGR);
}
