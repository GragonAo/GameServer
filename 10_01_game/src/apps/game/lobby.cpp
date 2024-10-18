#include "lobby.h"

#include "libserver/message_component.h"
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
    auto pMsgCallBack = new MessageCallBackFunction();
    AddComponent<MessageComponent>(pMsgCallBack);

    // 注册处理网络断开事件的回调函数，关联消息 ID 为 MI_NetworkDisconnect
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_NetworkDisconnect, BindFunP1(this, &Lobby::HandleNetworkDisconnect));

    // 注册处理通过 Token 登录的回调函数，关联消息 ID 为 C2G_LoginByToken
    pMsgCallBack->RegisterFunction(Proto::MsgId::C2G_LoginByToken, BindFunP1(this, &Lobby::HandleLoginByToken));
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

    // 输出玩家进入游戏的调试日志
    LOG_DEBUG("enter game. account:" << proto.account().c_str());

    // 构建登录响应协议，并设置返回码
    Proto::LoginByTokenRs protoLoginGameRs;
    protoLoginGameRs.set_return_code(Proto::LoginByTokenRs::LGRC_OK);

    // 发送登录成功的响应包给客户端
    MessageSystemHelp::SendPacket(Proto::MsgId::C2G_LoginByTokenRs, pPacket, protoLoginGameRs);
}
