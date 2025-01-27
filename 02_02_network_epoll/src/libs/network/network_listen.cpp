#include <iostream>

#include "network_listen.h"

bool NetworkListen::Listen(std::string ip, int port) {

  _masterSocket = CreateSocket();
  if (_masterSocket == INVALID_SOCKET)
    return false;

  sockaddr_in addr;
  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  ::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr);

  if (::bind(_masterSocket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) <
      0) {
    std::cout << "::bind failed. err:" << _sock_err() << std::endl;
    return false;
  }

  int backlog = 10;
#ifndef WIN32
  char *ptr;
  if ((ptr = getenv("LISTENQ")) != nullptr)
    backlog = atoi(ptr);
#endif

  if (::listen(_masterSocket, backlog) < 0) {
    std::cout << "::listen failed." << _sock_err() << std::endl;
    return false;
  }

#ifdef EPOLL
  std::cout << "epoll model" << std::endl;
  InitEpoll();
#else
  std::cout << "select model" << std::endl;
#endif

  return true;
}

int NetworkListen::Accept() {
  struct sockaddr socketClient;
  socklen_t socketLength = sizeof(socketClient);

  int rs = 0;
  while (true) {
    SOCKET socket = ::accept(_masterSocket, &socketClient, &socketLength);
    if (socket == INVALID_SOCKET)
      return rs;

    SetSocketOpt(socket);
    CreateConnectObj(socket);

    ++rs;
  }
}

#ifdef EPOLL

void NetworkListen::Update() {
  Epoll();

  if (_mainSocketEventIndex >= 0) {
    Accept();
  }
}

#else

void NetworkListen::Update() {
  Select();

  if (FD_ISSET(_masterSocket, &readfds)) {
    Accept();
  }
}

#endif
