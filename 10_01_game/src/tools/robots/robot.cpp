#include "robot.h"

#include "robot_state_login.h"
#include "robot_state_http.h"
#include "robot_state_game.h"
#include "global_robots.h"

#include "libserver/common.h"
#include "libserver/robot_state_type.h"
#include "libserver/yaml.h"
#include "libserver/entity_system.h"
#include "libserver/log4_help.h"
#include "libserver/message_system_help.h"
#include "libserver/message_component.h"
#include "libserver/update_component.h"

#include <json/reader.h>
#include "robot_component_gametoken.h"
#include "robot_component_login.h"

// 唤醒机器人并初始化其状态
void Robot::Awake(std::string account)
{
    Player::Awake(nullptr, account); // 调用父类的 Awake 方法，传入账户

    // 更新组件
    auto pUpdateComponent = AddComponent<UpdateComponent>();
    pUpdateComponent->UpdataFunction = BindFunP0(this, &Robot::Update); // 绑定更新函数

    // 消息回调
    auto pMsgCallBack = new MessageCallBackFunctionFilterObj<Robot>();
    AddComponent<MessageComponent>(pMsgCallBack);
    pMsgCallBack->GetPacketObject = [this](NetworkIdentify* pIdentify)
    {
        // 检查网络标识符以确定是否与当前对象匹配
        if (_objKey == pIdentify->GetObjectKey())
            return this;

        return static_cast<Robot*>(nullptr); // 返回空指针
    };

    // 注册消息处理函数
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::MI_NetworkConnected, BindFunP2(this, &Robot::HandleNetworkConnected));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::MI_NetworkDisconnect, BindFunP2(this, &Robot::HandleNetworkDisconnect));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::MI_HttpOuterResponse, BindFunP2(this, &Robot::HandleHttpOuterResponse));

    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::C2L_AccountCheckRs, BindFunP2(this, &Robot::HandleAccountCheckRs));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::L2C_PlayerList, BindFunP2(this, &Robot::HandlePlayerList));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::L2C_GameToken, BindFunP2(this, &Robot::HandleGameToken));
    pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::C2G_LoginByTokenRs, BindFunP2(this, &Robot::HandleLoginByTokenRs));

    // 初始化状态管理器，设置初始状态为 Http_Connecting
    InitStateTemplateMgr(RobotStateType::Http_Connecting);
}

// 将机器人返回池
void Robot::BackToPool()
{
    _account = ""; // 清空账户信息
    Player::BackToPool(); // 调用父类的 BackToPool 方法
}

// 更新机器人状态
void Robot::Update()
{
    UpdateState(); // 更新状态
}

// 网络断开处理
void Robot::NetworkDisconnect()
{
    if (_socketKey.Socket == INVALID_SOCKET) // 检查 socket 是否有效
        return;

    // 发送断开请求消息
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect, this);
    _socketKey.Clean(); // 清理 socket 信息
}

// 注册状态
void Robot::RegisterState()
{
    // 注册 HTTP 连接状态
    RegisterStateClass(RobotStateType::Http_Connecting, DynamicStateBind(RobotStateHttpConnecting));
    RegisterStateClass(RobotStateType::Http_Connected, DynamicStateBind(RobotStateHttpConnected));

    // 注册登录相关状态
    RegisterStateClass(RobotStateType::Login_Connecting, DynamicStateBind(RobotStateLoginConnecting));
    RegisterStateClass(RobotStateType::Login_Connected, DynamicStateBind(RobotStateLoginConnected));
    RegisterStateClass(RobotStateType::Login_Logined, DynamicStateBind(RobotStateLoginLogined));
    RegisterStateClass(RobotStateType::Login_SelectPlayer, DynamicStateBind(RobotStateLoginSelectPlayer));

    // 注册游戏相关状态
    RegisterStateClass(RobotStateType::Game_Connecting, DynamicStateBind(RobotStateGameConnecting));
    RegisterStateClass(RobotStateType::Game_Connected, DynamicStateBind(RobotStateGameConnected));
}

// 处理网络连接成功的消息
void Robot::HandleNetworkConnected(Robot* pRobot, Packet* pPacket)
{
    _socketKey = pPacket->GetSocketKey(); // 获取并存储 socket 关键信息
}

// 处理网络断开消息
void Robot::HandleNetworkDisconnect(Robot* pRobot, Packet* pPacket)
{
    // 检查网络类型是否匹配
    if (_socketKey.NetType != pPacket->GetSocketKey().NetType)
        return;

    _socketKey = { INVALID_SOCKET, NetworkType::None }; // 重置 socket 关键信息
}

// 处理 HTTP 响应消息
void Robot::HandleHttpOuterResponse(Robot* pRobot, Packet* pPacket)
{
    auto protoHttp = pPacket->ParseToProto<Proto::Http>(); // 解析 HTTP 消息
    const int code = protoHttp.status_code(); // 获取状态码

    if (code == 200) // 检查状态码是否为 200
    {
        const auto response = protoHttp.body(); // 获取响应体
        Json::CharReaderBuilder readerBuilder;
        Json::CharReader* jsonReader = readerBuilder.newCharReader();

        Json::Value value; // 用于存储解析的 JSON 数据
        JSONCPP_STRING errs; // 存储解析错误信息

        // 解析 JSON 数据
        const bool ok = jsonReader->parse(response.data(), response.data() + response.size(), &value, &errs);
        if (ok && errs.empty())
        {
            const auto code = value["returncode"].asInt(); // 获取返回码
            if (code == Proto::LoginHttpReturnCode::LHRC_OK) // 检查返回码是否为成功
            {
                // 添加登录组件
                AddComponent<RobotComponentLogin>(value["ip"].asString(), value["port"].asInt());
                ChangeState(RobotStateType::Login_Connecting); // 切换状态到登录连接中
            }
        }
        else
        {
            LOG_ERROR("json parse failed. " << response.c_str()); // 记录解析失败的日志
        }

        delete jsonReader; // 释放 JSON 读取器
    }
    else
    {
        LOG_ERROR("http response error:" << code << "\r\n" << protoHttp.body().c_str()); // 记录 HTTP 响应错误日志
    }
}

// 处理账户检查响应消息
void Robot::HandleAccountCheckRs(Robot* pRobot, Packet* pPacket)
{
    Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>(); // 解析账户检查响应

    // 检查账户一致性
    if (pRobot->GetAccount() != this->GetAccount())
    {
        LOG_ERROR("somethin error. pRobot != this");
    }

    // 如果当前机器人是唯一的，记录调试信息
    if (GlobalRobots::GetInstance()->GetRobotsCount() == 1)
        LOG_DEBUG("account check result account:" << _account.c_str() << " code:" << proto.return_code());

    // 检查返回码是否成功
    if (proto.return_code() == Proto::AccountCheckReturnCode::ARC_OK)
    {
        ChangeState(RobotStateType::Login_Logined); // 切换状态到登录已完成
    }
    else
    {
        // 获取返回码描述并记录错误日志
        const google::protobuf::EnumDescriptor* descriptor = Proto::AccountCheckReturnCode_descriptor();
        const auto name = descriptor->FindValueByNumber(proto.return_code())->name();
        std::cout << "account check failed. account:" << _account << " code:" << name.c_str() << std::endl;
    }
}

// 处理玩家列表消息
void Robot::HandlePlayerList(Robot* pRobot, Packet* pPacket)
{
    // 解析接收到的玩家列表包
    auto protoList = pPacket->ParseToProto <Proto::PlayerList>();

    // 获取随机数生成器
    const auto pRandomEngine = GetSystemManager()->GetRandomEngine();

    // 检查玩家列表是否为空
    if (protoList.player_size() == 0)
    {
        // 创建新的玩家
        Proto::CreatePlayer protoCreate;

        // 将账户名设置为玩家名称
        protoCreate.set_name(_account.c_str());

        // 随机生成性别
        std::uniform_int_distribution<int> disGender(0, 1);
        const int nGender = disGender(*pRandomEngine);
        protoCreate.set_gender(nGender == 1 ? Proto::Gender::male : Proto::Gender::female);

        // 发送创建玩家的消息
        MessageSystemHelp::SendPacket(Proto::MsgId::C2L_CreatePlayer, pPacket, protoCreate);
    }
    else
    {
        // 随机选择一个玩家
        std::uniform_int_distribution<int> disIndex(0, protoList.player_size() - 1);
        const int index = disIndex(*pRandomEngine);

        auto protoPlayer = protoList.player(index);

        // 选择玩家
        Proto::SelectPlayer protoSelect;
        protoSelect.set_player_sn(protoPlayer.sn());
        _playerSn = protoPlayer.sn();

        // 发送选择玩家的消息
        MessageSystemHelp::SendPacket(Proto::MsgId::C2L_SelectPlayer, pPacket, protoSelect);

        // 改变状态为选择玩家
        ChangeState(RobotStateType::Login_SelectPlayer);
    }
}

void Robot::HandleGameToken(Robot* pRobot, Packet* pPacket)
{
    // 解析游戏令牌包
    auto proto = pPacket->ParseToProto<Proto::GameToken>();
    LOG_DEBUG("HandleGameToken. token:" << proto.token().c_str() << " ip:" << proto.ip().c_str() << " port:" << proto.port());

    // 检查返回代码是否为游戏令牌成功
    if (proto.return_code() != Proto::GameToken_ReturnCode_GameToken_OK)
    {
        LOG_DEBUG("HandleLoginGame. not found game process.");
        return;
    }

    // 添加游戏令牌组件
    AddComponent<RobotComponentGameToken>(proto.token().c_str(), proto.ip().c_str(), proto.port());
    
    // 改变状态为游戏连接中
    ChangeState(RobotStateType::Game_Connecting);
}

void Robot::HandleLoginByTokenRs(Robot* pRobot, Packet* pPacket)
{
    // 解析登录令牌响应包
    auto protoRs = pPacket->ParseToProto<Proto::LoginByTokenRs>();

    // 检查登录是否成功
    if (protoRs.return_code() != Proto::LoginByTokenRs::LGRC_OK)
    {
        LOG_ERROR("login game failed. return_code:" << protoRs.return_code());
        return;
    }

    // 改变状态为游戏已登录
    ChangeState(RobotStateType::Game_Logined);
}
