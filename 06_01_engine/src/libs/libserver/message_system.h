#pragma once
#include "packet.h"
#include "message_list.h"
#include "system.h"

class IMessageSystem : virtual public ISystem {
public:
  ~IMessageSystem();
  virtual void RegisterMsgFunction() = 0;

  void AttachCallbackHandler(MessageCallBackFunctionInfo *pCallback);
  bool IsFollowMsgId(Packet *pPacket) const;
  void ProcessPacket(Packet *pPacket) const;
  static void DispatchPacket(Packet *pPacket);
  static void SendPacket(Packet *pPacket);

protected:
  MessageCallBackFunctionInfo *_pCallBackFuns{nullptr};
};