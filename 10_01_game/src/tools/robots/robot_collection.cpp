#include "robot_collection.h"
#include "libserver/message_system.h"

#include <json/reader.h>
#include "robot_component_login.h"
#include "global_robots.h"
#include "robot_component_gametoken.h"
#include "libserver/message_system_help.h"

// 唤醒函数，用于初始化消息系统并注册处理消息的回调函数
void RobotCollection::Awake()
{
    // 获取消息系统实例
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();

    // 注册网络连接和断开连接的处理函数
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::MI_NetworkConnected, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandleNetworkConnected));
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::MI_NetworkDisconnect, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandleNetworkDisconnect));
    
    // 注册HTTP外部响应处理函数
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::MI_HttpOuterResponse, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandleHttpOuterResponse));

    // 注册与账号相关的消息处理函数
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::C2L_AccountCheckRs, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandleAccountCheckRs));

    // 注册与玩家列表和游戏令牌相关的消息处理函数
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::L2C_PlayerList, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandlePlayerList));
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::L2C_GameToken, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandleGameToken));

    // 注册通过游戏令牌登录结果的处理函数
    pMsgSystem->RegisterFunctionFilter<Robot>(this, Proto::MsgId::C2G_LoginByTokenRs, 
                                              BindFunP1(this, &RobotCollection::GetMsgObj), 
                                              BindFunP2(this, &RobotCollection::HandleLoginByTokenRs));
}

// 回收到对象池时的处理函数，清除所有机器人实例
void RobotCollection::BackToPool()
{
    auto pEntitySystem = GetSystemManager()->GetEntitySystem();
    
    // 遍历并移除所有的机器人组件
    for (auto iter = _robots.begin(); iter != _robots.end(); ++iter)
    {
        pEntitySystem->RemoveComponent(iter->second);
    }

    // 清空机器人集合
    _robots.clear();
}

// 创建一个新的机器人并将其加入到集合中
void RobotCollection::CreateRobot(std::string account)
{
    auto pRobot = GetSystemManager()->GetEntitySystem()->AddComponent<Robot>(account);
    _robots.insert(std::make_pair(account, pRobot));
}

// 根据网络标识符找到对应的机器人对象
Robot* RobotCollection::GetMsgObj(NetworkIdentify* pIdentify)
{
    auto objKey = pIdentify->GetObjectKey();
    if (objKey.KeyType != ObjectKeyType::Account)
        return nullptr;

    auto iter = _robots.find(objKey.KeyValue.KeyStr);
    if (iter == _robots.end())
        return nullptr;

    return iter->second;
}

// 处理机器人连接网络的情况
void RobotCollection::HandleNetworkConnected(Robot* pRobot, Packet* pPacket)
{
    pRobot->SetSocketKey(pPacket->GetSocketKey());
}

// 处理机器人断开网络的情况
void RobotCollection::HandleNetworkDisconnect(Robot* pRobot, Packet* pPacket)
{
    // 确保当前断开的连接是正确的网络类型
    if (pRobot->GetSocketKey().NetType != pPacket->GetSocketKey().NetType)
        return;

    pRobot->SetSocketKey({ INVALID_SOCKET, NetworkType::None });
}

// 处理外部HTTP响应
void RobotCollection::HandleHttpOuterResponse(Robot* pRobot, Packet* pPacket)
{
    auto protoHttp = pPacket->ParseToProto<Proto::Http>();
    const int code = protoHttp.status_code();
    
    if (code == 200)
    {
        const auto response = protoHttp.body();
        Json::CharReaderBuilder readerBuilder;
        Json::CharReader* jsonReader = readerBuilder.newCharReader();
        Json::Value value;
        JSONCPP_STRING errs;

        // 解析HTTP响应的JSON内容
        const bool ok = jsonReader->parse(response.data(), response.data() + response.size(), &value, &errs);
        if (ok && errs.empty())
        {
            const auto code = value["returncode"].asInt();
            if (code == Proto::LoginHttpReturnCode::LHRC_OK)
            {
                pRobot->AddComponent<RobotComponentLogin>(value["ip"].asString(), value["port"].asInt());
                pRobot->ChangeState(RobotStateType::Login_Connecting);
            }
        }
        else
        {
            LOG_ERROR("json parse failed. " << response.c_str());
        }

        delete jsonReader;
    }
    else
    {
        LOG_ERROR("http response error:" << code << "\r\n" << protoHttp.body().c_str());
    }
}

// 处理账号检查的响应结果
void RobotCollection::HandleAccountCheckRs(Robot* pRobot, Packet* pPacket)
{
    Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>();
    if (GlobalRobots::GetInstance()->GetRobotsCount() == 1)
        LOG_DEBUG("account check result account:" << pRobot->GetAccount().c_str() << " code:" << proto.return_code());

    if (proto.return_code() == Proto::AccountCheckReturnCode::ARC_OK)
    {
        pRobot->ChangeState(RobotStateType::Login_Logined);
    }
    else
    {
        const google::protobuf::EnumDescriptor* descriptor = Proto::AccountCheckReturnCode_descriptor();
        const auto name = descriptor->FindValueByNumber(proto.return_code())->name();
        LOG_WARN("account check failed. account:" << pRobot->GetAccount().c_str() << " code:" << name.c_str());
    }
}

// 处理服务器返回的玩家列表
void RobotCollection::HandlePlayerList(Robot* pRobot, Packet* pPacket)
{
    auto protoList = pPacket->ParseToProto <Proto::PlayerList>();

    const auto pRandomEngine = GetSystemManager()->GetRandomEngine();

    if (protoList.player_size() == 0)
    {
        // 创建新玩家
        Proto::CreatePlayer protoCreate;
        protoCreate.set_name(pRobot->GetAccount().c_str());

        // 随机分配性别
        std::uniform_int_distribution<int> disGender(0, 1);
        const int nGender = disGender(*pRandomEngine);
        if (nGender == 1)
            protoCreate.set_gender(Proto::Gender::male);
        else
            protoCreate.set_gender(Proto::Gender::female);

        MessageSystemHelp::SendPacket(Proto::MsgId::C2L_CreatePlayer, pPacket, protoCreate);
    }
    else
    {
        // 随机选择一个玩家
        std::uniform_int_distribution<int> disIndex(0, protoList.player_size() - 1);
        const int index = disIndex(*pRandomEngine);
        auto protoPlayer = protoList.player(index);

        Proto::SelectPlayer protoSelect;
        protoSelect.set_player_sn(protoPlayer.sn());
        MessageSystemHelp::SendPacket(Proto::MsgId::C2L_SelectPlayer, pPacket, protoSelect);

        pRobot->ChangeState(RobotStateType::Login_SelectPlayer);
    }
}

// 处理游戏令牌的返回结果
void RobotCollection::HandleGameToken(Robot* pRobot, Packet* pPacket)
{
    auto proto = pPacket->ParseToProto<Proto::GameToken>();
    
    if (proto.return_code() != Proto::GameToken_ReturnCode_GameToken_OK)
    {
        LOG_DEBUG("HandleLoginGame. not found game process.");
        return;
    }

    // 为机器人添加游戏令牌组件，并切换到游戏连接状态
    pRobot->AddComponent<RobotComponentGameToken>(proto.token().c_str(), proto.ip().c_str(), proto.port());
    pRobot->ChangeState(RobotStateType::Game_Connecting);
}

// 处理通过令牌登录的响应结果
void RobotCollection::HandleLoginByTokenRs(Robot* pRobot, Packet* pPacket)
{
    auto protoRs = pPacket->ParseToProto<Proto::LoginByTokenRs>();
    
    if (protoRs.return_code() != Proto::LoginByTokenRs::LGRC_OK)
    {
        LOG_ERROR("login game failed. return_code:" << protoRs.return_code());
        return;
    }

    pRobot->ChangeState(RobotStateType::Game_Logined);
}
