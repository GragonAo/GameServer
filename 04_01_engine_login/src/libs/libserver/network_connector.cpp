#include "network_connector.h"

bool NetworkConnector::Init() { return true; }
bool NetworkConnector::Connect(std::string ip, int port) {
  _ip = ip;
  _port = port;
  if (_port == 0 || _ip == "")
    return false;
  _masterSocket = CreateSocket();
  if (_masterSocket == INVALID_SOCKET)
    return false;
#ifdef EPOLL
  std::cout << "epoll model" << std::endl;
  InitEpoll();
#else
  std::cout << "epoll model" << std::endl;
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
  if (IsConnected())
    return;
  if (_mainSocketEventIndex >= 0) {
    int fd = _events[_mainSocketEventIndex].data.fd;
    if (fd != _masterSocket)
      return;
    if(_events[_mainSocketEventIndex].events & EPOLLIN || _events[_mainSocketEventIndex].events & EPOLLOUT){
      TryCreateConnectObj();
    }
  }
  Network::Update();
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
      Dispose();
      return;
    }
    if (FD_ISSET(_masterSocket, &readfds) ||
        FD_ISSET(_masterSocket, &writefds)) {
      TryCreateConnectObj();
    }
  }

  Network::Update();
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
    Dispose();
  }
}