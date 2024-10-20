#pragma once
#include "app_type_mgr.h"
#include "common.h"
#include "protobuf/proto_id.pb.h"
#include "packet.h"
#include "message_list.h"
#include "system.h"
#include <google/protobuf/message.h>

class IMessageSystem : virtual public ISystem {
public:
  ~IMessageSystem();
  virtual void RegisterMsgFunction() = 0;

  void AttachCallbackHandler(MessageCallBackFunctionInfo *pCallback);
  bool IsFollowMsgId(Packet *pPacket) const;
  void ProcessPacket(Packet *pPacket) const;

  static Packet* CreatePacket(Proto::MsgId msgId,SOCKET socket);

  static void DispatchPacket(const Proto::MsgId msgId,const SOCKET socket);
  static void DispatchPacket(const Proto::MsgId msgId,const SOCKET socket,google::protobuf::Message& proto);

  static void SendPacket(const Proto::MsgId msgId,const SOCKET socket,google::protobuf::Message& proto);
  static void SendPacket(const Proto::MsgId msgId,google::protobuf::Message& proto,APP_TYPE appType,int appId = 0);

protected:
  static void DispatchPacket(Packet *pPacket);
  
  static void SendPacket(Packet* packet,APP_TYPE appType,int appId = 0);
  static void SendPacket(Packet *pPacket);

protected:
  MessageCallBackFunctionInfo *_pCallBackFuns{nullptr};
};