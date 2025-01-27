#include "robot.h"
#include "libserver/app_type_mgr.h"
#include "libserver/common.h"
#include "libserver/message_list.h"
#include "libserver/network_connector.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/state_template.h"
#include "libserver/yaml.h"
#include "robot_state_login.h"
#include "robot_state_type.h"

void Robot::AwakeFromPool(std::string account) {
  _account = account;
  _isBroadcast = false;
  InitStateTemplateMgr(RobotStateType::RobotState_Login_Connecting);

  auto pYaml = Yaml::GetInstance();
  const auto pLoginConfig =
      dynamic_cast<LoginConfig *>(pYaml->GetConfig(APP_LOGIN));

  this->Connect(pLoginConfig->Ip, pLoginConfig->Port);
}

void Robot::RegisterMsgFunction() {
  auto pMsgCallBack = new MessageCallBackFunctionFilterObj<Robot>();
  pMsgCallBack->GetPacketObject = [this](SOCKET socket) -> Robot * {
    if (this->GetSocket() == socket)
      return this;

    return nullptr;
  };

  AttachCallbackHandler(pMsgCallBack);

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::C2L_AccountCheckRs,
      BindFunP2(this, &Robot::HandleAccountCheckRs));
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

  auto pPacket = new Packet(Proto::MsgId::C2L_AccountCheck, GetSocket());
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
}

void Robot::HandleAccountCheckRs(Robot *pRobot, Packet *pPacket) {
  Proto::AccountCheckRs proto = pPacket->ParseToProto<Proto::AccountCheckRs>();
  std::cout << "account check result account:" << _account
            << " code:" << proto.return_code() << std::endl;

  if (proto.return_code() == Proto::AccountCheckRs::ARC_OK)
    ChangeState(RobotStateType::RobotState_Login_Logined);
}