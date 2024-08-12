#include "message_system.h"
#include "thread_mgr.h"
#include "network_locator.h"
IMessageSystem::~IMessageSystem() {
  if (_pCallBackFuns != nullptr) {
    delete _pCallBackFuns;
    _pCallBackFuns = nullptr;
  }
}
void IMessageSystem::AttachCallbackHandler(
    MessageCallBackFunctionInfo *pCallback) {
  _pCallBackFuns = pCallback;
}
bool IMessageSystem::IsFollowMsgId(Packet *pPacket) const{
  if (_pCallBackFuns == nullptr)
    return false;
  return _pCallBackFuns->IsFollowMsgId(pPacket);
}
void IMessageSystem::ProcessPacket(Packet *pPacket) const{
  if (_pCallBackFuns == nullptr)
    return;
  _pCallBackFuns->ProcessPacket(pPacket);
}
void IMessageSystem::DispatchPacket(Packet *pPacket) {
  ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}
void IMessageSystem::SendPacket(Packet *pPacket) {
  auto pNetworkLocator =
      ThreadMgr::GetInstance()->GetComponent<NetworkLocator>();
  auto pNetwork = pNetworkLocator->GetNetworkConnector(pPacket->GetSocket());
  if (pNetwork != nullptr) {
    pNetwork->SendPacket(pPacket);
    return;
  }
  if (Global::GetInstance()->GetCurAppType() == APP_ALL &&
      pPacket->GetSocket() == 0) {
    DispatchPacket(pPacket);
    return;
  }

  pNetwork = pNetworkLocator->GetListen(NetworkTcpListen);
  pNetwork->SendPacket(pPacket);
}