#include "account.h"
#include "http_request_account.h"
#include "libserver/common.h"
#include "libserver/packet.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/thread_mgr.h"
#include "login_obj.h"

bool Account::Init() { return true; }
void Account::Update() {}
void Account::RegisterMsgFunction() {
  auto pMsgCallBack = new MessageCallBackFunction();
  AttachCallBackHandler(pMsgCallBack);
  pMsgCallBack->RegisterFunction(Proto::MsgId::C2L_AccountCheck,
                                 BindFunP1(this, &Account::HandleAccountCheck));
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_AccountCheckToHttpRs,
      BindFunP1(this, &Account::HandleAccountCheckToHttpRs));
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_NetworkDisconnect,
      BindFunP1(this, &Account::HandleNetworkDisconnect));
}

void Account::HandleNetworkDisconnect(Packet* pPacket)
{
    //std::cout << "player offline. socket:" << pPacket->GetSocket() << std::endl;
    _playerMgr.RemovePlayer(pPacket->GetSocket());
}
void Account::HandleAccountCheck(Packet *pPacket) {
  auto protoCheck = pPacket->ParseToProto<Proto::AccountCheck>();
  const auto socket = pPacket->GetSocket();
  auto pPlayer = _playerMgr.GetPlayer(protoCheck.account());
  if (pPlayer != nullptr) {
    Proto::AccountCheckRs protoResult;
    protoResult.set_return_code(Proto::AccountCheckRs::ARC_LOGGING);
    auto pRsPacket = new Packet(Proto::MsgId::C2L_AccountCheckRs, socket);
    pRsPacket->SerializeToBuffer(protoResult);
    SendPacket(pRsPacket);
    const auto pPacketDis =
        new Packet(Proto::MsgId::MI_NetworkDisconnectToNet, socket);
    DisPatchPacket(pPacketDis);
    std::cout << pPlayer->GetAccount() << " 用户已经登录" << std::endl;
    return;
  }
  _playerMgr.AddPlayer(socket, protoCheck.account(), protoCheck.password());
  HttpRequestAccount *pHttp =
      new HttpRequestAccount(protoCheck.account(), protoCheck.password());
  ThreadMgr::GetInstance()->AddObjToThread(pHttp);
}

void Account::HandleAccountCheckToHttpRs(Packet *pPacket) {
  auto proto = pPacket->ParseToProto<Proto::AccountCheckToHttpRs>();

  auto *pPlayer = _playerMgr.GetPlayer(proto.account());
  if (pPlayer == nullptr) {
    std::cout << "can't find player. account:" << proto.account().c_str()
              << std::endl;
    return;
  }

  // std::cout << "account check result account:" << proto.account()
  //           << " rs:" << proto.return_code()
  //           << " socket:" << pPlayer->GetSocket() << std::endl;
  Proto::AccountCheckRs protoResult;
  protoResult.set_return_code(proto.return_code());
  auto pResultPacket =
      new Packet(Proto::MsgId::C2L_AccountCheckRs, pPlayer->GetSocket());
  pResultPacket->SerializeToBuffer(protoResult);
  if (pResultPacket)
    SendPacket(pResultPacket);

  if (proto.return_code() != Proto::AccountCheckRs::ARC_OK) {
    _playerMgr.RemovePlayer(pPlayer->GetSocket());
  }
}