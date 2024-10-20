#include "robot.h"
#include "global_robots.h"
#include "libserver/common.h"
#include "libserver/log4_help.h"
#include "libserver/message_callback.h"
#include "libserver/message_component.h"
#include "libserver/message_system.h"
#include "libserver/message_system_help.h"
#include "libserver/network_connector.h"
#include "libserver/network_type.h"
#include "libserver/protobuf/db.pb.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/socket_object.h"
#include "libserver/state_template.h"
#include "libserver/update_component.h"
#include "robot_state_login.h"
#include "robot_state_type.h"
#include "robot_state_http.h"
#include <json/reader.h>
#include <random>
#include <sstream>
#include <thread>

void Robot::Awake(std::string account) {
  _account = account;
  _objKey = {ObjectKeyType::Account, {0, _account}};

  auto pUpdateComponent = AddComponent<UpdateComponent>();
  pUpdateComponent->UpdateFunction = BindFunP0(this, &Robot::Update);

  auto pMsgCallBack = new MessageCallBackFunctionFilterObj<Robot>();
  AddComponent<MessageComponent>(pMsgCallBack);
  pMsgCallBack->GetPacketObject = [this](NetworkIdentify *pIdentify) {
    if (_objKey == pIdentify->GetObjectKey())
      return this;
    return static_cast<Robot *>(nullptr);
  };

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::MI_NetworkConnected,
      BindFunP2(this, &Robot::HandleNetworkConnected));
  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::MI_NetworkDisconnect,
      BindFunP2(this, &Robot::HandleNetworkDisconnect));
  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::MI_HttpOuterResponse,
      BindFunP2(this, &Robot::HandleHttpOuterResponse));

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::C2L_AccountCheckRs,
      BindFunP2(this, &Robot::HandleAccountCheckRs));

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::L2C_PlayerList, BindFunP2(this, &Robot::HandlePlayerList));

  InitStateTemplateMgr(RobotStateType::Http_Connecting);
}

void Robot::BackToPool() { _account = ""; }

void Robot::Update() { UpdateState(); }

std::string Robot::GetAccount() const { return _account; }

void Robot::RegisterState() {

  RegisterStateClass(RobotStateType::Http_Connecting, DynamicStateBind(RobotStateHttpConnecting));
  RegisterStateClass(RobotStateType::Http_Connected, DynamicStateBind(RobotStateHttpConnected));

  RegisterStateClass(RobotStateType::Login_Connecting,
                     DynamicStateBind(RobotStateLoginConnecting));
  RegisterStateClass(RobotStateType::Login_Connected,
                     DynamicStateBind(RobotStateLoginConnected));
  RegisterStateClass(RobotStateType::Login_Logined,
                     DynamicStateBind(RobotStateLoginLogined));
  RegisterStateClass(RobotStateType::Login_SelectPlayer,
                     DynamicStateBind(RobotStateLoginSelectPlayer));
}

void Robot::HandleNetworkConnected(Robot *pRobot, Packet *pPacket) {
  if (_socketKey.NetType != NetworkType::None &&
      _socketKey.Socket != INVALID_SOCKET) {
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkRequestDisconnect,
                                      this);
  }
  _socketKey = pPacket->GetSocketKey();
}

void Robot::HandleNetworkDisconnect(Robot *pRobot, Packet *pPacket) {
  if (_socketKey.NetType != pPacket->GetSocketKey().NetType) {
    return;
  }
  _socketKey = {INVALID_SOCKET, NetworkType::None};
}

void Robot::HandleHttpOuterResponse(Robot *pRobot, Packet *pPacket) {
  auto protoHttp = pPacket->ParseToProto<Proto::Http>();
  const int code = protoHttp.status_code();
  if (code == 200) {
    const auto response = protoHttp.body();
    Json::CharReaderBuilder readerBuilder;
    Json::CharReader *jsonReder = readerBuilder.newCharReader();

    Json::Value value;
    JSONCPP_STRING errs;
    const bool ok = jsonReder->parse(
        response.data(), response.data() + response.size(), &value, &errs);
    if (ok && errs.empty()) {
      const auto code = value["code"].asInt();
      if (code == Proto::LoginHttpreturnCode::LHRC_OK) {
        _loginIp = value["ip"].asString();
        _loginPort = value["port"].asInt();
        ChangeState(RobotStateType::Login_Connecting);
      } else {
        LOG_ERROR("json parse failed. " << response.c_str());
      }
      delete jsonReder;
    } else {
      LOG_ERROR("http response error: " << code << "\r\n"
                                        << protoHttp.body().c_str());
    }
  }
}

void Robot::ConnectToLogin() const {
  Proto::NetworkConnect protoConn;
  protoConn.set_network_type((int)NetworkType::TcpConnector);
  _objKey.SerializeToProto(protoConn.mutable_key());
  protoConn.set_ip(_loginIp.c_str());
  protoConn.set_port(_loginPort);
  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkConnect, protoConn,
                                    nullptr);
}

void Robot::HandleAccountCheckRs(Robot *pRobot, Packet *pPacket) {
  Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>();

  if (pRobot->GetAccount() != this->GetAccount()) {
    LOG_ERROR("somethin error. pRobot != this");
  }

  if (GlobalRobots::GetInstance()->GetRobotsCount() == 1) {
    LOG_DEBUG("account check result account: " << _account.c_str() << " code : "
                                               << proto.return_code());
  }

  if (proto.return_code() == Proto::AccountCheckReturnCode::ARC_OK)
    ChangeState(RobotStateType::Login_Logined);
  else {
    const google::protobuf::EnumDescriptor *descriptor =
        Proto::AccountCheckReturnCode_descriptor();
    const auto name =
        descriptor->FindValueByNumber(proto.return_code())->name();
    std::cout << "account check failed. account:" << _account
              << " code:" << proto.return_code() << std::endl;
  }
}

void Robot::HandlePlayerList(Robot *pRobot, Packet *pPacket) {
  auto protoList = pPacket->ParseToProto<Proto::PlayerList>();

  std::stringstream strStream;
  strStream << std::this_thread::get_id();
  std::string idstr = strStream.str();
  std::seed_seq seed1(idstr.begin(), idstr.end());
  std::minstd_rand0 generator(seed1);
  std::default_random_engine randomEngine(generator);

  if (protoList.player_size() == 0) {
    Proto::CreatePlayer protoCreate;

    protoCreate.set_name(_account.c_str());

    std::uniform_int_distribution<int> disGender(0, 1);
    const int nGender = disGender(randomEngine);
    if (nGender == 1) {
      protoCreate.set_gender(Proto::Gender::male);
    } else {
      protoCreate.set_gender(Proto::Gender::female);
    }
    MessageSystemHelp::SendPacket(Proto::MsgId::C2L_CreatePlayer, pPacket,
                                  protoCreate);
  } else {
    if (GlobalRobots::GetInstance()->GetRobotsCount() == 1)
      LOG_DEBUG("recv players. size:" << protoList.player_size());

    ChangeState(RobotStateType::Login_SelectPlayer);
  }
}
