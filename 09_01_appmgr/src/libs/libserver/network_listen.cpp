#include "network_listen.h"
#include "component_help.h"
#include "connect_obj.h"
#include "global.h"
#include "network_interface.h"
#include "log4_help.h"
#include "message_component.h"
#include "network.h"
#include "network_locator.h"
#include "thread_mgr.h"
#include "update_component.h"

void NetworkListen::Awake(std::string ip, int port, NetworkType iType) {

  auto pNetworkLocator = ThreadMgr::GetInstance()
                             ->GetEntitySystem()
                             ->GetComponent<NetworkLocator>();
  pNetworkLocator->AddListenLocator(this, iType);

  // message
  auto pMsgCallBack = new MessageCallBackFunction();
  AddComponent<MessageComponent>(pMsgCallBack);
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_NetworkRequestDisconnect,
      BindFunP1(this, &NetworkListen::HandleDisconnect));

  // update
  auto pUpdateComponent = AddComponent<UpdateComponent>();
  pUpdateComponent->UpdateFunction = BindFunP0(this, &NetworkListen::Update);

  _masterSocket = CreateSocket();
  if (_masterSocket == INVALID_SOCKET)
    return;

  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);

  if (::bind(_masterSocket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) <
      0) {
    LOG_ERROR("::bind failed. err:" << _sock_err());
    return;
  }
  if (::listen(_masterSocket, SOMAXCONN) < 0) {
    LOG_ERROR("::listen failed." << _sock_err());
  }
#ifdef EPOLL
  LOG_INFO("epoll model. listen " << ip.c_str() << ":" << port);
  InitEpoll();
#else
  LOG_INFO("select model. listen " << ip.c_str() << ":" << port);
#endif
  return;
}

void NetworkListen::Awake(int appType, int appId) {
  auto pGlobal = Global::GetInstance();
  auto pYaml = ComponentHelp::GetYaml();
  auto pCommonConfig = pYaml->GetIPEndPoint(pGlobal->GetCurAppType(),pGlobal->GetCurAppId());
  Awake(pCommonConfig->Ip, pCommonConfig->Port, NetworkType::TcpListen);
}

void NetworkListen::Awake(std::string ip, int port) {
  Awake(ip, port, NetworkType::HttpListen);
}

#ifdef EPOLL
void NetworkListen::Update() {
  Epoll();
  if (_mainSocketEventIndex >= 0) {
    Accept();
  }
  Network::OnNetworkUpdate();
}
#else
void NetworkListen::Update() {
  Select();
  if (FD_ISSET(_masterSocket, &readfds)) {
    Accept();
  }
  Network::OnNetworkUpdate();
}
#endif

int NetworkListen::Accept() {
  struct sockaddr socketClient;
  socklen_t socketLength = sizeof(socketClient);
  int rs = 0;
  while (true) {
    const SOCKET socket = ::accept(_masterSocket, &socketClient, &socketLength);
    if (socket == INVALID_SOCKET)
      return rs;
    // std::cout << "accept socket:"<<socket << std::endl;
    SetSocketOpt(socket);
    CreateConnectObj(socket);

    ++rs;
  }
}

const char *NetworkListen::GetTypeName() {
  return typeid(NetworkListen).name();
}

void NetworkListen::HandleDisconnect(Packet *pPacket) {
  auto socket = pPacket->GetSocket();
  auto iter = _connects.find(socket);
  if (iter == _connects.end()) {
    std::cout << "dis connect failed. socket not find. socket:" << socket
              << std::endl;
    return;
  }
  
  RemoveConnectObj(iter);
  std::cout << "logical layer requires shutdown. socket:" << socket
            << std::endl;
}