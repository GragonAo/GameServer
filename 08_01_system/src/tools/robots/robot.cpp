#include "robot.h"
#include "global_robots.h"
#include "libserver/app_type.h"
#include "libserver/common.h"
#include "libserver/component_help.h"
#include "libserver/log4_help.h"
#include "libserver/message_callback.h"
#include "libserver/message_component.h"
#include "libserver/message_system.h"
#include "libserver/message_system_help.h"
#include "libserver/network_connector.h"
#include "libserver/protobuf/db.pb.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/state_template.h"
#include "libserver/update_component.h"
#include "libserver/util_string.h"
#include "libserver/yaml.h"
#include "robot_state_login.h"
#include "robot_state_type.h"
#include <random>
#include <sstream>
#include <thread>

void Robot::Awake(std::string account) {
  _account = account;
  _isBroadcast = false;
  InitStateTemplateMgr(RobotStateType::RobotState_Login_Connecting);

  auto pMsgCallBack = new MessageCallBackFunctionFilterObj<Robot>();
  AddComponent<MessageComponent>(pMsgCallBack);
  pMsgCallBack->GetPacketObject = [this](SOCKET socket) -> Robot * {
    if (this->GetSocket() == socket)
      return this;

    return nullptr;
  };

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::C2L_AccountCheckRs,
      BindFunP2(this, &Robot::HandleAccountCheckRs));

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::L2C_PlayerList, BindFunP2(this, &Robot::HandlePlayerList));

  auto pUpdateComponent = AddComponent<UpdateComponent>();
  pUpdateComponent->UpdateFunction = BindFunP0(this, &Robot::Update);

  auto pYaml = ComponentHelp::GetYaml();
  const auto pLoginConfig =
      dynamic_cast<LoginConfig *>(pYaml->GetConfig(APP_LOGIN));

  this->Connect(pLoginConfig->Ip, pLoginConfig->Port);
}

void Robot::Update() {
  NetworkConnector::Update();
  UpdateState();
}

std::string Robot::GetAccount() const { return _account; }

void Robot::SendMsgAccountCheck() {
  Proto::AccountCheck accountCheck;
  accountCheck.set_account(GetAccount());
  accountCheck.set_password("e10adc3949ba59abbe56e057f20f883e");

  auto pPacket = MessageSystemHelp::CreatePacket(Proto::MsgId::C2L_AccountCheck,
                                                 GetSocket());
  pPacket->SerializeToBuffer(accountCheck);
  SendPacket(pPacket);
}

void Robot::RegisterState() {
  RegisterStateClass(RobotStateType::RobotState_Login_Connecting,
                     DynamicStateBind(RobotStateLoginConnecting));
  RegisterStateClass(RobotStateType::RobotState_Login_Connected,
                     DynamicStateBind(RobotStateLoginConnected));
  RegisterStateClass(RobotStateType::RobotState_Login_Logined,
                     DynamicStateBind(RobotStateLoginLogined));
  RegisterStateClass(RobotStateType::RobotState_Login_SelectPlayer,
                     DynamicStateBind(RobotStateLoginSelectPlayer));
}

void Robot::HandleAccountCheckRs(Robot *pRobot, Packet *pPacket) {
  Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>();

  // std::cout << "account check result account:" << _account
  //           << " code:" << proto.return_code() << std::endl;

  if (proto.return_code() == Proto::AccountCheckReturnCode::ARC_OK)
    ChangeState(RobotStateType::RobotState_Login_Logined);
  else {
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

    std::uniform_int_distribution<int> disName(1000, 10000 - 1);
    const int nNameRandom = disName(randomEngine);
    std::string playerName =
        strutil::format("%s-%d", _account.c_str(), nNameRandom);

    protoCreate.set_name(playerName.c_str());

    std::uniform_int_distribution<int> disGender(0, 1);
    const int nGender = disGender(randomEngine);
    if (nGender == 1) {
      protoCreate.set_gender(Proto::Gender::male);
    } else {
      protoCreate.set_gender(Proto::Gender::female);
    }
    LOG_DEBUG("create. name: " << playerName.c_str()
                               << " gender: " << (int)Proto::Gender::male);

    Packet *pPacketCreate = MessageSystemHelp::CreatePacket(
        Proto::MsgId::C2L_CreatePlayer, GetSocket());
    pPacketCreate->SerializeToBuffer(protoCreate);
    SendPacket(pPacketCreate);
  } else {
    if (GlobalRobots::GetInstance()->GetRobotsCount() == 1)
      LOG_DEBUG("recv players. size:" << protoList.player_size());

    ChangeState(RobotStateType::RobotState_Login_SelectPlayer);
  }
}
