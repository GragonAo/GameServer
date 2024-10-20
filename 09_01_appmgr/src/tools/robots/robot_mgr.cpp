#include "robot_mgr.h"
#include "global_robots.h"
#include "libserver/common.h"
#include "libserver/entity_system.h"
#include "libserver/message_component.h"
#include "libserver/message_system_help.h"
#include "libserver/packet.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "robot_state_type.h"
#include <algorithm>
#include <chrono>
#include <sstream>

void RobotMgr::Awake() {
  auto pMsgCallBack = new MessageCallBackFunction();
  AddComponent<MessageComponent>(pMsgCallBack);
  pMsgCallBack->RegisterFunction(Proto::MsgId::MI_RobotSyncState,
                                 BindFunP1(this, &RobotMgr::HandleRobotState));

  AddTimer(0, 2, false, 0, BindFunP0(this, &RobotMgr::ShowInfo));
}

void RobotMgr::BackToPool(){}

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
  auto curTime = timeutil::ToString(Global::GetInstance()->TimeTick);
  show << "+++++++++++++++++++++++++++++++++++++=" << std::endl;
  
  for (RobotStateType rss = RobotStateType::Http_Connecting; rss < RobotStateType::End;
       rss = static_cast<RobotStateType>(rss + 1)) {
    if (stateData.find(rss) == stateData.end()) {
      show << GetRobotStateTypeName(rss) << 0 << std::endl;
    } else {
      show << GetRobotStateTypeName(rss) << stateData[rss] << std::endl;
    }
  }
  show << "+++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << show.str().c_str() << std::endl;
}

void RobotMgr::HandleRobotState(Packet *pPacket) {
  Proto::RobotSyncState protoState =
      pPacket->ParseToProto<Proto::RobotSyncState>();

  if (_robots.empty() && protoState.states_size() > 0) {
    std::cout << "test begin" << std::endl;
    _start = std::chrono::system_clock::now();
  }

  RobotStateType iType = RobotStateType::Space_EnterWorld;
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
    auto end = std::chrono::system_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - _start);
    auto stateName = GetRobotStateTypeName(iType);

    std::cout << stateName << " over. time:"
              << double(duration.count()) *
                     std::chrono::microseconds::period::num /
                     std::chrono::microseconds::period::den
              << "s" << std::endl;
  }
}