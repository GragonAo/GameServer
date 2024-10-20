#include "account.h"
#include "libserver/app_type.h"
#include "libserver/common.h"
#include "libserver/component_help.h"
#include "libserver/global.h"
#include "libserver/log4_help.h"
#include "libserver/message_component.h"
#include "libserver/message_system.h"
#include "libserver/message_system_help.h"
#include "libserver/network_type.h"
#include "libserver/packet.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/socket_object.h"
#include "libserver/yaml.h"
#include "login_obj.h"
#include <json/config.h>
#include <json/reader.h>

void Account::Awake() {

  auto pYaml = ComponentHelp::GetYaml();
  const auto pLoginConfig =
      dynamic_cast<LoginConfig *>(pYaml->GetConfig(APP_LOGIN));
  ParseUrlInfo info;
  if (!MessageSystemHelp::ParseUrl(pLoginConfig->UrlLogin, info)) {
    LOG_ERROR(
        "parse login url failed. url: " << pLoginConfig->UrlLogin.c_str());
  } else {
    _httpIp = info.Host;
    _httpPort = info.Port;
    _method = info.Mothed;
  }

  AddTimer(0, 10, true, 2, BindFunP0(this, &Account::SyncAppInfoToAppMgr));

  auto pMsgCallBack = new MessageCallBackFunction();
  AddComponent<MessageComponent>(pMsgCallBack);

  pMsgCallBack->RegisterFunction(Proto::MsgId::C2L_AccountCheck,
                                 BindFunP1(this, &Account::HandleAccountCheck));

  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_HttpOuterResponse,
      BindFunP1(this, &Account::HandleHttpOuterResponse));
  pMsgCallBack->RegisterFunction(Proto::MsgId::C2L_CreatePlayer,
                                 BindFunP1(this, &Account::HandleCreatePlayer));
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::L2DB_CreatePlayerRs,
      BindFunP1(this, &Account::HandleCreatePlayerRs));
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::L2DB_QueryPlayerListRs,
      BindFunP1(this, &Account::HandleQueryPlayerListRs));

  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_NetworkDisconnect,
      BindFunP1(this, &Account::HandleNetworkDisconnect));
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_NetworkConnected,
      BindFunP1(this, &Account::HandleNetworkConnected));
}

void Account::BackToPool() { _playerMgr.BackToPool(); }

void Account::SyncAppInfoToAppMgr() {
  Proto::AppInfoSync protoSync;
  protoSync.set_app_id(Global::GetInstance()->GetCurAppId());
  protoSync.set_app_type((int)Global::GetInstance()->GetCurAppType());
  protoSync.set_online(_playerMgr.Count());
  MessageSystemHelp::SendPacket(Proto::MsgId::MI_AppInfoSync, protoSync,
                                APP_APPMGR);
}

void Account::HandleNetworkConnected(Packet *pPacket) {
  auto connectKey = pPacket->GetObjectKey();

  if (connectKey.KeyType != ObjectKeyType::Account)
    return;

  if (pPacket->GetSocketKey().NetType != NetworkType::HttpConnector)
    return;

  auto pPlayer = _playerMgr.GetPlayer(connectKey.KeyValue.KeyStr);
  if (pPlayer == nullptr) {
    LOG_ERROR("http connected. but can't find player. account: "
              << pPlayer->GetAccount().c_str() << pPlayer);
    return;
  }
#ifdef LOG_TRACE_COMPONENT_OPEN
  ComponentHelp::GetTraceComponent()->TraceAccount(
      pPlayer->GetAccount(), pPacket->GetSocketKey().Socket);
#endif

  NetworkIdentify httpIndentify{pPacket->GetSocketKey(),
                                pPlayer->GetObjectKey()};
  std::map<std::string, std::string> params;
  params["account"] = pPlayer->GetAccount();
  params["password"] = pPlayer->GetPassword();
  MessageSystemHelp::SendHttpRequest(&httpIndentify, _httpIp, _httpPort,
                                     _method, &params);
}

void Account::HandleNetworkDisconnect(Packet *pPacket) {
  if (pPacket->GetObjectKey().KeyType != ObjectKeyType::Account)
    return;

  const auto socketKey = pPacket->GetSocketKey();
  if (socketKey.NetType != NetworkType::TcpListen)
    return;
  _playerMgr.RemovePlayer(pPacket->GetSocketKey().Socket);
}

void Account::SocketDisconnect(std::string account,
                               NetworkIdentify *pIdentify) {
  Proto::AccountCheckRs protoResult;
  protoResult.set_return_code(Proto::AccountCheckReturnCode::ARC_LOGGING);
  MessageSystemHelp::SendPacket(Proto::MsgId::C2L_AccountCheckRs, pIdentify,
                                protoResult);

  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect,
                                    pIdentify);
}

void Account::HandleAccountCheck(Packet *pPacket) {
  auto protoCheck = pPacket->ParseToProto<Proto::AccountCheck>();
  const auto socket = pPacket->GetSocketKey();

  auto pPlayer = _playerMgr.GetPlayer(protoCheck.account());
  if (pPlayer != nullptr) {
    LOG_WARN("account check failed. same account: "
             << protoCheck.account().c_str() << ". " << pPlayer);
    SocketDisconnect(protoCheck.account(), pPacket);
    return;
  }

  pPlayer = _playerMgr.AddPlayer(pPacket, protoCheck.account(),
                                 protoCheck.password());

  if (pPacket == nullptr) {
    LOG_WARN("account check failed. same socket. " << pPacket);
    SocketDisconnect(protoCheck.account(), pPacket);
    return;
  }

#ifdef LOG_TRACE_COMPONENT_OPEN
  ComponentHelp::GetTraceComponent()->TraceAccount(protoCheck.account(),
                                                   socket.Socket);
#endif

  Proto::NetworkConnect protoConn;
  protoConn.set_network_type((int)NetworkType::HttpConnector);
  pPlayer->GetObjectKey().SerializeToProto(protoConn.mutable_key());
  protoConn.set_ip(_httpIp.c_str());
  protoConn.set_port(_httpPort);
  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkConnect, protoConn,
                                    nullptr);
}

void Account::HandleQueryPlayerListRs(Packet *pPacket) {
  auto protoRs = pPacket->ParseToProto<Proto::PlayerList>();
  std::string account = protoRs.account();

  auto pPlayer = _playerMgr.GetPlayer(account);
  if (pPlayer == nullptr) {
    LOG_ERROR("HandleQueryPlayerLists. pPlayer == nullptr. account: "
              << account.c_str());
    return;
  }
  MessageSystemHelp::SendPacket(Proto::MsgId::L2C_PlayerList, pPlayer, protoRs);
}

void Account::HandleCreatePlayer(Packet *pPacket) {
  auto protoCreate = pPacket->ParseToProto<Proto::CreatePlayer>();
  auto pPlayer = _playerMgr.GetPlayer(pPacket->GetSocketKey().Socket);
  if (pPlayer == nullptr) {
    LOG_ERROR("HandleCreatePlayer. pPlayer == nullptr. socket: "
              << pPacket->GetSocketKey().Socket);
    return;
  }

  std::string account = pPlayer->GetAccount();
  LOG_DEBUG("create. account: " << account.c_str()
                                << "name: " << protoCreate.name().c_str()
                                << "gender:" << (int)protoCreate.gender());

  Proto::CreatePlayerToDB proto2Db;
  proto2Db.set_account(account.c_str());

  auto pProtoPlayer = proto2Db.mutable_player();
  pProtoPlayer->set_sn(Global::GetInstance()->GenerateSN());
  pProtoPlayer->set_name(protoCreate.name());
  pProtoPlayer->mutable_base()->set_gender(protoCreate.gender());
  MessageSystemHelp::SendPacket(Proto::MsgId::L2DB_CreatePlayer, proto2Db,
                                APP_DB_MGR);
}

void Account::HandleCreatePlayerRs(Packet *pPacket) {
  auto protoRs = pPacket->ParseToProto<Proto::CreatePlayerToDBRs>();
  std::string account = protoRs.account();

  auto pPlayer = _playerMgr.GetPlayer(pPacket->GetSocketKey().Socket);
  if (pPlayer == nullptr) {
    LOG_ERROR("HandleCreatePlayerToDBRs. pPlayer == nullptr. account: "
              << pPlayer->GetAccount());
    return;
  }

  Proto::CreatePlayerRs createProto;
  createProto.set_return_code(protoRs.return_code());

  MessageSystemHelp::SendPacket(Proto::MsgId::C2L_CreatePlayerRs, pPlayer,
                                createProto);
}

void Account::HandleHttpOuterResponse(Packet *pPacket) {
  auto connectKey = pPacket->GetObjectKey();
  auto pPlayer = _playerMgr.GetPlayer(connectKey.KeyValue.KeyStr);
  if (pPlayer == nullptr) {
    LOG_ERROR("http out response. but can't find player. account: "
              << connectKey.KeyValue.KeyStr.c_str());
    return;
  }

  auto protoHttp = pPacket->ParseToProto<Proto::Http>();
  Proto::AccountCheckReturnCode rsCode =
      Proto::AccountCheckReturnCode::ARC_TIMEOUT;
  int statusCode = protoHttp.status_code();
  if (statusCode == 200) {
    auto response = protoHttp.body();
    const Json::CharReaderBuilder readerBuilder;
    Json::CharReader *jsonReader = readerBuilder.newCharReader();

    Json::Value value;
    JSONCPP_STRING errs;

    const bool ok = jsonReader->parse(
        response.data(), response.data() + response.size(), &value, &errs);
    if (ok && errs.empty()) {
      rsCode = ProcessMsg(value);
    } else {
      LOG_ERROR("json parse failed. " << response.c_str());
    }
    delete jsonReader;
  }
  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect,
                                    pPacket);

  if (rsCode == Proto::AccountCheckReturnCode::ARC_OK) {
    Proto::QueryPlayerList protoQuery;
    protoQuery.set_account(pPlayer->GetAccount().c_str());
    MessageSystemHelp::SendPacket(Proto::MsgId::L2DB_QueryPlayerList,
                                  protoQuery, APP_DB_MGR);
  } else {
    Proto::AccountCheckRs protoResult;
    protoResult.set_return_code(rsCode);
    MessageSystemHelp::SendPacket(Proto::MsgId::C2L_AccountCheckRs, pPlayer,
                                  protoResult);
  }
}

Proto::AccountCheckReturnCode Account::ProcessMsg(Json::Value value) const {
  Proto::AccountCheckReturnCode code =
      Proto::AccountCheckReturnCode::ARC_UNKONWN;
  const int httpcode = value["code"].asInt();
  if (httpcode == 0) {
    code = Proto::AccountCheckReturnCode::ARC_OK;
  } else if (httpcode == 2) {
    code = Proto::AccountCheckReturnCode::ARC_NOT_FOUND_ACCOUNT;
  }else if(httpcode == 3)
    code = Proto::AccountCheckReturnCode::ARC_PASSWORD_WRONG;
  return code;
}