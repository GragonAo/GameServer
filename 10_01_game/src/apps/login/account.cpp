#include "account.h"
#include "libserver/common.h"
#include "libserver/thread_mgr.h"
#include "libserver/log4_help.h"
#include "libserver/message_component.h"
#include "libserver/message_system_help.h"
#include "libserver/component_help.h"

#include "libserver/global.h"
#include "libplayer/player_collector_component.h"
#include "player_component_account.h"
#include "libplayer/player_component_proto_list.h"

void Account::Awake()
{
    // 添加玩家收集器组件，用于管理玩家列表
    AddComponent<PlayerCollectorComponent>();

    // 从配置文件读取 HTTP 登录配置
    auto pYaml = ComponentHelp::GetYaml();
    const auto pLoginConfig = dynamic_cast<LoginConfig*>(pYaml->GetConfig(APP_LOGIN));
    ParseUrlInfo info;
    if (!MessageSystemHelp::ParseUrl(pLoginConfig->UrlLogin, info)) // 解析登录 URL
    {
        LOG_ERROR("parse login url failed. url:" << pLoginConfig->UrlLogin.c_str());
    }
    else
    {
        // 存储解析出的 IP 地址、端口号和 HTTP 方法
        _httpIp = info.Host;
        _httpPort = info.Port;
        _method = pLoginConfig->UrlMethod;
        _path = info.Path;
    }

    // 设置定时器，每 10 秒同步一次应用信息到 App 管理器
    AddTimer(0, 10, true, 2, BindFunP0(this, &Account::SyncAppInfoToAppMgr));

    // 注册消息回调，用于处理各种消息
    auto pMsgCallBack = new MessageCallBackFunction();
    AddComponent<MessageComponent>(pMsgCallBack);

    // 注册回调函数以处理账号检查请求
    pMsgCallBack->RegisterFunction(Proto::MsgId::C2L_AccountCheck, BindFunP1(this, &Account::HandleAccountCheck));

    // 注册回调函数以处理外部 HTTP 响应
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_HttpOuterResponse, BindFunP1(this, &Account::HandleHttpOuterResponse));

    // 注册回调函数以处理数据库返回的玩家列表查询结果
    pMsgCallBack->RegisterFunction(Proto::MsgId::L2DB_QueryPlayerListRs, BindFunP1(this, &Account::HandleQueryPlayerListRs));

    // 处理网络断开连接事件
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_NetworkDisconnect, BindFunP1(this, &Account::HandleNetworkDisconnect));

    // 处理网络连接事件
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_NetworkConnected, BindFunP1(this, &Account::HandleNetworkConnected));

    // 处理应用信息同步
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_AppInfoListSync, BindFunP1(this, &Account::HandleAppInfoListSync));

    // 处理玩家创建和选择
    pMsgCallBack->RegisterFunction(Proto::MsgId::C2L_CreatePlayer, BindFunP1(this, &Account::HandleCreatePlayer));
    pMsgCallBack->RegisterFunction(Proto::MsgId::L2DB_CreatePlayerRs, BindFunP1(this, &Account::HandleCreatePlayerRs));
    pMsgCallBack->RegisterFunction(Proto::MsgId::C2L_SelectPlayer, BindFunP1(this, &Account::HandleSelectPlayer));
}

void Account::BackToPool()
{
    // 清空应用列表
    _apps.clear();
}

void Account::SyncAppInfoToAppMgr()
{
    // 同步当前应用的信息到 App 管理器
    Proto::AppInfoSync protoSync;
    protoSync.set_app_id(Global::GetInstance()->GetCurAppId());
    protoSync.set_app_type((int)Global::GetInstance()->GetCurAppType());
    protoSync.set_online(GetComponent<PlayerCollectorComponent>()->OnlineSize());

    // 发送同步消息
    MessageSystemHelp::SendPacket(Proto::MsgId::MI_AppInfoSync, protoSync, APP_APPMGR);
}

void Account::HandleAppInfoListSync(Packet* pPacket)
{
    // 处理 App 信息同步消息
    auto proto = pPacket->ParseToProto<Proto::AppInfoListSync>();
    for (auto one : proto.apps())
    {
        Parse(one, INVALID_SOCKET);
    }
}

void Account::HandleNetworkConnected(Packet* pPacket)
{
    // 处理网络连接事件，检查是否为 HTTP 连接并找到对应的玩家
    auto objKey = pPacket->GetObjectKey();
    if (objKey.KeyType != ObjectKeyType::Account)
        return;

    if (pPacket->GetSocketKey().NetType != NetworkType::HttpConnector)
        return;

    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    auto pPlayer = pPlayerCollector->GetPlayerByAccount(objKey.KeyValue.KeyStr);
    if (pPlayer == nullptr)
    {
        LOG_ERROR("http connected. but can't find player. account:" << objKey.KeyValue.KeyStr.c_str() << pPacket);
        return;
    }

    const auto pPlayerCAccount = pPlayer->GetComponent<PlayerComponentAccount>();

    // 发送 HTTP 验证请求
    NetworkIdentify httpIndentify{ pPacket->GetSocketKey(), pPlayer->GetObjectKey() };
    std::map<std::string, std::string> params;
    params["account"] = pPlayer->GetAccount();
    params["password"] = pPlayerCAccount->GetPassword();
    MessageSystemHelp::SendHttpRequest(&httpIndentify, _httpIp, _httpPort, _method,_path, &params);
}

void Account::HandleNetworkDisconnect(Packet* pPacket)
{
    // 处理网络断开事件
    const auto socketKey = pPacket->GetSocketKey();
    if (socketKey.NetType != NetworkType::TcpListen)
        return;

    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();
    pPlayerCollector->RemovePlayerBySocket(pPacket->GetSocketKey().Socket);
}

void Account::SocketDisconnect(std::string account, NetworkIdentify* pIdentify)
{
    // 处理 socket 断开连接并通知客户端
    Proto::AccountCheckRs protoResult;
    protoResult.set_return_code(Proto::AccountCheckReturnCode::ARC_LOGGING);
    MessageSystemHelp::SendPacket(Proto::MsgId::C2L_AccountCheckRs, pIdentify, protoResult);

    // 关闭连接
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect, pIdentify);
}

void Account::HandleAccountCheck(Packet* pPacket)
{
    // 处理账号检查消息
    auto protoCheck = pPacket->ParseToProto<Proto::AccountCheck>();
    const auto socket = pPacket->GetSocketKey();

    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    // 检查是否有同一账号登录
    auto pPlayer = pPlayerCollector->GetPlayerByAccount(protoCheck.account());
    if (pPlayer != nullptr)
    {
        LOG_WARN("account check failed. same account:" << protoCheck.account().c_str() << ". " << pPlayer);
        SocketDisconnect(protoCheck.account(), pPacket);
        return;
    }

    // 检查是否有同一 socket 登录
    pPlayer = pPlayerCollector->AddPlayer(pPacket, protoCheck.account());
    if (pPlayer == nullptr)
    {
        LOG_WARN("account check failed. same socket. " << pPacket);
        SocketDisconnect(protoCheck.account(), pPacket);
        return;
    }

    pPlayer->AddComponent<PlayerComponentAccount>(protoCheck.password());

#ifdef LOG_TRACE_COMPONENT_OPEN
    ComponentHelp::GetTraceComponent()->TraceAccount(protoCheck.account(), socket.Socket);
#endif

    // 创建 HTTP 连接以验证账号信息
    MessageSystemHelp::CreateConnect(NetworkType::HttpConnector, pPlayer->GetObjectKey(), _httpIp.c_str(), _httpPort);
}

void Account::HandleQueryPlayerListRs(Packet* pPacket)
{
    // 处理从数据库返回的玩家列表
    auto protoRs = pPacket->ParseToProto<Proto::PlayerList>();
    std::string account = protoRs.account();

    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    const auto pPlayer = pPlayerCollector->GetPlayerByAccount(account);
    if (pPlayer == nullptr)
    {
        LOG_ERROR("HandleQueryPlayerLists. pPlayer == nullptr. account:" << account.c_str());
        return;
    }

    if (protoRs.player_size() > 0)
    {
        // 将玩家列表解析并存储到玩家组件中
        const auto pListObj = pPlayer->AddComponent<PlayerComponentProtoList>();
        pListObj->Parse(protoRs);
    }

    // 将玩家列表发送到客户端
    MessageSystemHelp::SendPacket(Proto::MsgId::L2C_PlayerList, pPlayer, protoRs);
}

// 处理创建玩家的请求
void Account::HandleCreatePlayer(Packet* pPacket)
{
    // 获取玩家收集器组件
    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    // 从数据包中解析出创建玩家的协议数据
    auto protoCreate = pPacket->ParseToProto<Proto::CreatePlayer>();
    // 根据Socket信息查找对应的玩家
    const auto pPlayer = pPlayerCollector->GetPlayerBySocket(pPacket->GetSocketKey().Socket);
    if (pPlayer == nullptr)
    {
        // 如果没有找到对应的玩家，输出错误日志并返回
        LOG_ERROR("HandleCreatePlayer. pPlayer == nullptr. socket:" << pPacket->GetSocketKey().Socket);
        return;
    }

    // 获取玩家的账号信息
    std::string account = pPlayer->GetAccount();
    // 输出创建玩家的调试信息，包括账号、名称和性别
    LOG_DEBUG("create. account:" << account.c_str() << " name:" << protoCreate.name().c_str() << " gender:" << (int)protoCreate.gender());

    // 创建发往数据库的创建玩家协议数据包
    Proto::CreatePlayerToDB proto2Db;
    proto2Db.set_account(account.c_str()); // 设置账号信息

    // 填充玩家基本信息
    auto pProtoPlayer = proto2Db.mutable_player();
    pProtoPlayer->set_sn(Global::GetInstance()->GenerateSN()); // 生成唯一的SN
    pProtoPlayer->set_name(protoCreate.name()); // 设置玩家名称
    pProtoPlayer->mutable_base()->set_gender(protoCreate.gender()); // 设置玩家性别

    // 发送创建玩家的请求到数据库管理器
    MessageSystemHelp::SendPacket(Proto::MsgId::L2DB_CreatePlayer, proto2Db, APP_DB_MGR);
}

// 处理创建玩家的响应
void Account::HandleCreatePlayerRs(Packet* pPacket)
{
    // 从数据包中解析出数据库返回的创建玩家响应协议数据
    auto protoRs = pPacket->ParseToProto<Proto::CreatePlayerToDBRs>();
    // 获取账号信息
    std::string account = protoRs.account();

    // 获取玩家收集器组件
    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    // 根据Socket信息查找对应的玩家
    auto pPlayer = pPlayerCollector->GetPlayerBySocket(pPacket->GetSocketKey().Socket);
    if (pPlayer == nullptr)
    {
        // 如果没有找到对应的玩家，输出错误日志并返回
        LOG_ERROR("HandleCreatePlayerToDBRs. pPlayer == nullptr. account:" << account.c_str());
        return;
    }

    // 创建客户端响应协议包，并设置返回码
    Proto::CreatePlayerRs createProto;
    createProto.set_return_code(protoRs.return_code());

    // 发送创建玩家的响应给客户端
    MessageSystemHelp::SendPacket(Proto::MsgId::C2L_CreatePlayerRs, pPlayer, createProto);
}

// 处理选择玩家的请求
void Account::HandleSelectPlayer(Packet* pPacket)
{
    // 创建默认的选择玩家响应协议，并设置成功返回码
    Proto::SelectPlayerRs protoRs;
    protoRs.set_return_code(Proto::SelectPlayerRs::SPRC_OK);

    // 解析客户端发来的选择玩家请求协议数据
    auto proto = pPacket->ParseToProto<Proto::SelectPlayer>();
    // 获取玩家收集器组件
    auto pPlayerMgr = GetComponent<PlayerCollectorComponent>();
    // 根据Socket查找玩家
    auto pPlayer = pPlayerMgr->GetPlayerBySocket(pPacket->GetSocketKey().Socket);
    if (pPlayer == nullptr)
    {
        // 如果没有找到玩家，输出错误日志并返回
        LOG_ERROR("HandleSelectPlayer. pPlayer == nullptr");
        return;
    }

    // 获取客户端请求选择的玩家SN
    uint64 selectPlayerSn = proto.player_sn();

    // 获取玩家登录信息组件，并设置选中的玩家SN
    auto pPlayerLoginInfo = pPlayer->GetComponent<PlayerComponentAccount>();
    pPlayerLoginInfo->SetSelectPlayerSn(selectPlayerSn);

    do
    {
        if (pPlayer == nullptr)
        {
            // 如果没有找到玩家，设置错误返回码，并输出日志
            protoRs.set_return_code(Proto::SelectPlayerRs::SPRC_NotFound);
            LOG_ERROR("HandleSelectPlayer. pPlayer == nullptr. " << pPacket);
            break;
        }

        // 获取玩家的协议列表组件
        auto pSubCompoent = pPlayer->GetComponent<PlayerComponentProtoList>();
        if (pSubCompoent == nullptr)
        {
            // 如果玩家协议组件不存在，设置错误返回码，并输出日志
            protoRs.set_return_code(Proto::SelectPlayerRs::SPRC_NotFound);
            LOG_ERROR("HandleSelectPlayer. pPlayer == nullptr. " << pPacket);
            break;
        }

        // 从协议组件中获取对应SN的玩家数据流
        std::stringstream* pOpStream = pSubCompoent->GetProto(selectPlayerSn);
        if (pOpStream == nullptr)
        {
            // 如果找不到对应SN的玩家数据，设置错误返回码，并输出日志
            protoRs.set_return_code(Proto::SelectPlayerRs::SPRC_NotFound);
            LOG_ERROR("HandleSelectPlayer. can't find player sn:" << selectPlayerSn);
            break;
        }

        // 将数据流中的玩家信息解析并设置到玩家对象中
        pPlayer->ParseFromStream(selectPlayerSn, pOpStream);
    } while (false);

    // 如果选择玩家的返回码不是成功，则直接返回客户端错误信息
    if (Proto::SelectPlayerRs::SPRC_OK != protoRs.return_code())
    {
        MessageSystemHelp::SendPacket(Proto::MsgId::C2L_SelectPlayerRs, pPlayer, protoRs);
        return;
    }

    // 准备发送游戏Token给客户端
    Proto::GameToken protoToken;
    AppInfo info;
    if (!GetOneApp(APP_GAME, &info))
    {
        // 如果没有可用的游戏服务器，返回无游戏状态
        protoToken.set_return_code(Proto::GameToken_ReturnCode_GameToken_NO_GAME);
    }
    else
    {
        // 设置游戏服务器的IP和端口，并返回成功
        protoToken.set_return_code(Proto::GameToken_ReturnCode_GameToken_OK);
        protoToken.set_ip(info.Ip.c_str());
        protoToken.set_port(info.Port);
        protoToken.set_token(""); // 可以在这里生成和设置Token
    }

    // 将游戏Token发送给客户端
    MessageSystemHelp::SendPacket(Proto::MsgId::L2C_GameToken, pPlayer, protoToken);
}

// 处理HTTP外部响应
void Account::HandleHttpOuterResponse(Packet* pPacket)
{
    // 获取玩家收集器组件
    auto pPlayerCollector = GetComponent<PlayerCollectorComponent>();

    // 从数据包中获取对象键值，作为查找玩家的依据
    auto objKey = pPacket->GetObjectKey();
    // 根据对象键值中的账号查找对应的玩家
    auto pPlayer = pPlayerCollector->GetPlayerByAccount(objKey.KeyValue.KeyStr);
    if (pPlayer == nullptr)
    {
        // 如果没有找到对应的玩家，输出错误日志并返回
        LOG_ERROR("http out response. but can't find player. account:" << objKey.KeyValue.KeyStr.c_str() << pPacket);
        return;
    }

    // 从数据包中解析出HTTP协议的数据
    auto protoHttp = pPacket->ParseToProto<Proto::Http>();
    // 定义初始的账号验证返回码，默认为超时（ARC_TIMEOUT）
    Proto::AccountCheckReturnCode rsCode = Proto::AccountCheckReturnCode::ARC_TIMEOUT;
    int statusCode = protoHttp.status_code();
    
    std::cout <<"code: " << protoHttp.status_code() << "\nbody: "<<protoHttp.body()<<std::endl;

    // 如果HTTP状态码为200，表示请求成功
    if (statusCode == 200)
    {
        // 获取HTTP响应体内容
        auto response = protoHttp.body();
        // 使用JsonCPP库解析JSON内容
        const Json::CharReaderBuilder readerBuilder;
        Json::CharReader* jsonReader = readerBuilder.newCharReader();

        Json::Value value;
        JSONCPP_STRING errs;

        // 解析JSON字符串，如果解析成功且无错误
        const bool ok = jsonReader->parse(response.data(), response.data() + response.size(), &value, &errs);
        if (ok && errs.empty())
        {
            // 调用ProcessMsg方法处理JSON内容，返回相应的验证结果
            rsCode = ProcessMsg(value);
        }
        else
        {
            // 如果解析失败，输出错误日志
            LOG_ERROR("json parse failed. " << response.c_str());
        }

        // 释放Json解析器对象
        delete jsonReader;
    }

    // 处理完请求后，发送网络请求断开消息
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect, pPacket);

    // 如果账号验证成功，向数据库查询玩家列表
    if (rsCode == Proto::AccountCheckReturnCode::ARC_OK)
    {
        Proto::QueryPlayerList protoQuery;
        protoQuery.set_account(pPlayer->GetAccount().c_str());
        MessageSystemHelp::SendPacket(Proto::MsgId::L2DB_QueryPlayerList, protoQuery, APP_DB_MGR);
    }
    else
    {
        // 如果验证失败，构造验证失败的响应协议并发送给客户端
        Proto::AccountCheckRs protoResult;
        protoResult.set_return_code(rsCode);
        MessageSystemHelp::SendPacket(Proto::MsgId::C2L_AccountCheckRs, pPlayer, protoResult);
    }
}

// 处理HTTP响应中的JSON消息，返回相应的验证结果
Proto::AccountCheckReturnCode Account::ProcessMsg(Json::Value value) const
{
    // 初始返回码为未知错误
    Proto::AccountCheckReturnCode code = Proto::AccountCheckReturnCode::ARC_UNKONWN;
    // 获取返回码对应的HTTP返回值
    const int httpcode = value["returncode"].asInt();

    // 根据返回的HTTP返回码设置相应的账号验证结果
    if (httpcode == 0)
        code = Proto::AccountCheckReturnCode::ARC_OK; // 验证成功
    else if (httpcode == 2)
        code = Proto::AccountCheckReturnCode::ARC_NOT_FOUND_ACCOUNT; // 未找到账号
    else if (httpcode == 3)
        code = Proto::AccountCheckReturnCode::ARC_PASSWORD_WRONG; // 密码错误

    return code;
}
