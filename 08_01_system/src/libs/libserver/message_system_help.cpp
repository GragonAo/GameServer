#include "message_system_help.h"
#include "packet.h"
#include "thread_mgr.h"
#include "network_locator.h"
#include "log4_help.h"

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
  auto pNetworkLocator =
      ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
  auto pNetwork = pNetworkLocator->GetNetworkConnector(appType, appId);

  if (pNetwork != nullptr) {
    packet->SetSocket(pNetwork->GetSocket());
    pNetwork->SendPacket(packet);
    return;
  }

  if ((Global::GetInstance()->GetCurAppType() & appType) != 0) {
    DispatchPacket(packet);
  } else {
    LOG_ERROR("can't find network. appType:"
              << AppTypeMgr::GetInstance()->GetAppName(appType).c_str()
              << " appId:" << appId);
  }
}

void MessageSystemHelp::SendPacket(Packet *pPacket) {
  auto pNetworkLocator =
      ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
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

Packet *MessageSystemHelp::CreatePacket(Proto::MsgId msgId, SOCKET socket) {
  auto pPacket = new Packet(msgId, socket);
  return pPacket;
}