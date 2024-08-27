#include "message_system_help.h"
#include "global.h"
#include "log4_help.h"
#include "network_locator.h"
#include "packet.h"
#include "thread_mgr.h"
#include "object_pool_packet.h"

void MessageSystemHelp::DispatchPacket(Packet *pPacket) {
  ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}

void MessageSystemHelp::DispatchPacket(const Proto::MsgId msgId,
                                       const SOCKET socket) {
  const auto pPacket = CreatePacket(msgId, socket);
  DispatchPacket(pPacket);
}

void MessageSystemHelp::DispatchPacket(const Proto::MsgId msgId,
                                       const SOCKET socket,
                                       google::protobuf::Message &proto) {
  const auto pPacket = CreatePacket(msgId, socket);
  pPacket->SerializeToBuffer(proto);
  DispatchPacket(pPacket);
}

void MessageSystemHelp::SendPacket(const Proto::MsgId msgId,
                                   const SOCKET socket,
                                   google::protobuf::Message &proto) {
  const auto pPacket = CreatePacket(msgId, socket);
  pPacket->SerializeToBuffer(proto);
  SendPacket(pPacket);
}

void MessageSystemHelp::SendPacket(const Proto::MsgId msgId,
                                   google::protobuf::Message &proto,
                                   APP_TYPE appType, int appId) {
  auto packet = CreatePacket(msgId, 0);
  packet->SerializeToBuffer(proto);
  SendPacket(packet, appType, appId);
}

void MessageSystemHelp::SendPacket(Packet *packet, APP_TYPE appType,
                                   int appId) {

  if ((Global::GetInstance()->GetCurAppType() & appType) != 0) {
    //正好在当前进程中，直接转发，例如curapptype == all 的时候
    DispatchPacket(packet);
  } else {
    auto pNetworkLocator = ThreadMgr::GetInstance()
                               ->GetEntitySystem()
                               ->GetComponent<NetworkLocator>();
    auto pNetwork = pNetworkLocator->GetNetworkConnector(appType, appId);

    if (pNetwork != nullptr) {
      packet->SetSocket(pNetwork->GetSocket());
      pNetwork->SendPacket(packet);
    } else {
      LOG_ERROR("can't find network. appType:"
                << GetAppName(appType)
                << " appId:" << appId);
    }
  }
}

void MessageSystemHelp::SendPacket(Packet *pPacket) {

  if(pPacket->GetSocket() == 0){
    DispatchPacket(pPacket);
    return;
  }

  auto pNetworkLocator = ThreadMgr::GetInstance()
                             ->GetEntitySystem()
                             ->GetComponent<NetworkLocator>();
  auto pNetwork = pNetworkLocator->GetNetworkConnector(pPacket->GetSocket());
  if (pNetwork != nullptr) {
    pNetwork->SendPacket(pPacket);
    return;
  }

  pNetwork = pNetworkLocator->GetListen(TcpListen);
  pNetwork->SendPacket(pPacket);
}

Packet *MessageSystemHelp::CreatePacket(Proto::MsgId msgId, SOCKET socket) {
  return DynamicPacketPool::GetInstance()->MallocPacket(msgId, socket);
}