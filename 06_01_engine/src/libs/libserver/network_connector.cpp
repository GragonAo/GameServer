#include "network_connector.h"
#include "network.h"

const char* NetworkConnector::GetTypeName(){
  return typeid(NetworkConnector).name();
}

void NetworkConnector::AwakeFromPool(std::string ip,int port){
  Connect(ip,port);
}

bool NetworkConnector::Connect(std::string ip, int port) {
  _ip = ip;
  _port = port;

  if (_port == 0 || _ip == "")
    return false;

  _masterSocket = CreateSocket();
  if (_masterSocket == INVALID_SOCKET)
    return false;

#ifdef EPOLL
  InitEpoll();
#endif

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_port);
  ::inet_pton(AF_INET, _ip.c_str(), &addr.sin_addr.s_addr);

  int rs = ::connect(_masterSocket, (struct sockaddr *)&addr, sizeof(sockaddr));
  if (rs == 0) {
    CreateConnectObj(_masterSocket);
  }
  return true;
}
#ifdef EPOLL
void NetworkConnector::Update() {
  if (_masterSocket == INVALID_SOCKET) {
    if (!Connect(_ip, _port))
      return;
    std::cout << "re connect. socket" << _masterSocket << std::endl;
  }
  Epoll();

  if (!IsConnected()){
      if (_mainSocketEventIndex >= 0) {
    int fd = _events[_mainSocketEventIndex].data.fd;
    if (fd != _masterSocket)
      return;
    if(_events[_mainSocketEventIndex].events & EPOLLIN || _events[_mainSocketEventIndex].events & EPOLLOUT){
      TryCreateConnectObj();
    }
  }
  }

  Network::OnNetworkUpdate();
}
#else
void NetworkConnector::Update() {
  if (_masterSocket == INVALID_SOCKET) {
    if (!Connect(_ip, _port))
      return;
    std::cout << "re connect. socket:" << _masterSocket << std::endl;
  }
  Select();
  if (!IsConnected()) {
    if (FD_ISSET(_masterSocket, &exceptfds)) {
      Clean();
      return;
    }
    if (FD_ISSET(_masterSocket, &readfds) ||
        FD_ISSET(_masterSocket, &writefds)) {
      TryCreateConnectObj();
    }
  }

  Network::OnNetworkUpdate();
}
#endif

bool NetworkConnector::IsConnected() const { return _connects.size() > 0; }

void NetworkConnector::TryCreateConnectObj() {
  int optval = -1;
  socklen_t optlen = sizeof(optval);
  int rs = ::getsockopt(_masterSocket, SOL_SOCKET, SO_ERROR, (char *)(&optval),
                        &optlen);
  if (rs == 0 && optval == 0) {
    CreateConnectObj(_masterSocket);
  } else {
    std::cout << "connect failed. socket:" << _masterSocket << std::endl;
    Clean();
  }
}