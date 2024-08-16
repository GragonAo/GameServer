#include "robot_mgr.h"
#include "global_robots.h"
#include "libserver/common.h"
#include "libserver/entity_system.h"
#include "libserver/global.h"
#include "libserver/message_component.h"
#include "libserver/message_system_help.h"
#include "libserver/packet.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/update_component.h"
#include "libserver/yaml.h"
#include "robot_state_type.h"
#include <algorithm>
#include <sstream>

void RobotMgr::AwakeFromPool() {

  auto pUpdateComponent = AddComponent<UpdateComponent>();
  pUpdateComponent->UpdateFunction = BindFunP0(this, &RobotMgr::Update);

  auto pMsgCallBack = new MessageCallBackFunction();
  AddComponent<MessageComponent>(pMsgCallBack);
  pMsgCallBack->RegisterFunction(Proto::MsgId::MI_RobotSyncState,
                                 BindFunP1(this, &RobotMgr::HandleRobotState));

  auto pYaml = Yaml::GetInstance();
  const auto pLoginConfig =
      dynamic_cast<LoginConfig *>(pYaml->GetConfig(APP_LOGIN));
  this->Connect(pLoginConfig->Ip, pLoginConfig->Port);
}

void RobotMgr::Update() {
  NetworkConnector::Update();
  if (_nextShowInfoTime > Global::GetInstance()->TimeTick)
    return;
  _nextShowInfoTime = timeutil::AddSeconds(Global::GetInstance()->TimeTick, 2);
  ShowInfo();
}

void RobotMgr::ShowInfo() {
  if (!_isChange)
    return;
  _isChange = false;
  std::map<RobotStateType, int> stateData;
  std::for_each(_robots.cbegin(), _robots.cend(), [&stateData](auto one) {
    auto state = one.second;
    if (stateData.find(state) == stateData.end()) {
      stateData[state] = 0;
    }
    stateData[state]++;
  });
  std::stringstream show;
  show << "+++++++++++++++++++++++++++++++++++++=" << std::endl;
  for (RobotStateType rss = RobotState_HttpRequest; rss < RobotState_End;
       rss = static_cast<RobotStateType>(rss + 1)) {
    if (stateData.find(rss) == stateData.end()) {
      show << GetRobotStatetypeShortName(rss) << 0 << std::endl;
    } else {
      show << GetrobotStateTypeName(rss) << stateData[rss] << std::endl;
    }
  }
  show << "+++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << show.str().c_str() << std::endl;
}

void RobotMgr::HandleRobotState(Packet *pPacket) {
  Proto::RobotSyncState protoState =
      pPacket->ParseToProto<Proto::RobotSyncState>();
  if (_robots.size() == 0 && protoState.states_size() > 0) {
    std::cout << "test begin" << std::endl;
    _nextShowInfoTime = 0;
    Packet *pPacketBegin =
        new Packet(Proto::MsgId::MI_RobotTestBegin, GetSocket());
    SendPacket(pPacketBegin);
  }
  RobotStateType iType = RobotState_Space_EnterWorld;
  for (int index = 0; index < protoState.states_size(); index++) {
    auto proto = protoState.states(index);
    const auto account = proto.account();
    _robots[account] = RobotStateType(proto.state());
    if (_robots[account] < iType) {
      iType = _robots[account];
    }
  }
  _isChange = true;
  NofityServer(iType);
}

void RobotMgr::NofityServer(RobotStateType iType) {
  if (_robots.size() != GlobalRobots::GetInstance()->GetRobotsCount())
    return;

  auto iter = std::find_if(_robots.begin(), _robots.end(), [&iType](auto pair) {
    if (pair.second < iType)
      return true;

    return false;
  });

  if (iter == _robots.end()) {
    std::cout << "test over " << GetRobotStatetypeShortName(iType) << std::endl;
    ;
    Packet *pPacketEnd = new Packet(Proto::MsgId::MI_RobotTestEnd, GetSocket());
    Proto::RobotTestEnd protoEnd;
    protoEnd.set_state(iType);
    pPacketEnd->SerializeToBuffer(protoEnd);
    SendPacket(pPacketEnd);
  }
}