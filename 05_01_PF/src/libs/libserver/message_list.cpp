#include "message_list.h"
#include "packet.h"
#include "thread_mgr.h"

void MessageCallBackFunction::RegisterFunction(int msgId,
                                               HandleFunction function) {
  _callbackHandle[msgId] = function;
}

bool MessageCallBackFunction::IsFollowMsgId(Packet *packet) {
  return _callbackHandle.find(packet->GetMsgId()) != _callbackHandle.end();
}

void MessageCallBackFunction::ProcessPacket(Packet *packet) {
  const auto handleIter = _callbackHandle.find(packet->GetMsgId());
  if (handleIter == _callbackHandle.end()) {
    std::cout << "packet is not hander. msg id" << packet->GetMsgId()
              << std::endl;
  } else {
    handleIter->second(packet);
  }
}

void MessageList::Dispose() {
  if (_pCallBackFuns != nullptr) {
    delete _pCallBackFuns;
    _pCallBackFuns = nullptr;
  }
}

void MessageList::AttachCallBackHandler(
    MessageCallBackFunctionInfo *pCallback) {
  _pCallBackFuns = pCallback;
}

bool MessageList::IsFollowMsgId(Packet *packet) const {
  if (_pCallBackFuns == nullptr) {
    return false;
  }
  return _pCallBackFuns->IsFollowMsgId(packet);
}

void MessageList::ProcessPacket(Packet *packet) const {
  if (_pCallBackFuns == nullptr)
    return;
  _pCallBackFuns->ProcessPacket(packet);
}

void MessageList::DisPatchPacket(Packet *pPakcet) {
  ThreadMgr::GetInstance()->DispatchPacket(pPakcet);
}

void MessageList::SendPacket(Packet *pPacket) {
  ThreadMgr::GetInstance()->SendPacket(pPacket);
}
