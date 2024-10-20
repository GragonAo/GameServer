#include "network.h"
#include "connect_obj.h"
#include "packet.h"
// 重写 Dispose 方法，释放资源
void Network::Dispose() {
  auto iter = _connects.begin();
  while (iter != _connects.end()) {
    iter->second->Dispose();
    delete iter->second;
    ++iter;
  }
  _connects.clear();
#ifdef EPOLL
  ::close(_epfd);
#endif
  _sock_close(_masterSocket);
  _masterSocket = -1;
}

// 重写 RegisterMsgFunction 方法，注册消息处理函数
void Network::RegisterMsgFunction() {
  RegisterFunction(Proto::MsgId::MI_NetworkDisconnectToNet,
                   BindFunP1(this, &Network::HandleDisconnect));
}
// 发送数据包
void Network::SendPacket(Packet *pPacket) {
  if (pPacket == nullptr) {
    std::cerr << "SendPacket received a nullptr" << std::endl;
    return;
  }
  std::lock_guard<std::mutex> guard(_sendMsgMutex);
  _sendMsgList.push_back(pPacket);
}

#define SetsockOptType void *
// 设置套接字选项
void Network::SetSocketOpt(SOCKET socket) {
  bool isRuseaddr = true;
  setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (SetsockOptType)&isRuseaddr,
             sizeof(isRuseaddr));
  int netTimeout = 3000;
  setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (SetsockOptType)&netTimeout,
             sizeof(netTimeout));
  setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (SetsockOptType)&netTimeout,
             sizeof(netTimeout));
  int keepAlive = 1;
  socklen_t optlen = sizeof(keepAlive);
  int keepIdle = 60 * 2;
  int keepInterval = 10;
  int keepCount = 5;
  setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (SetsockOptType)&keepAlive,
             optlen);
  if (getsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, &optlen) < 0) {
    std::cout << "getsockopt SO_KEEPALIVE failed." << std::endl;
  }
  setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, optlen);
  if (getsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, &optlen) <
      0) {
    std::cout << "getsockopt TCP_KEEPIDLE failed." << std::endl;
  }
  setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, optlen);
  setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, optlen);
  _sock_nonblock(socket);
}

// 创建套接字
SOCKET Network::CreateSocket() {
  _sock_init();
  SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket == INVALID_SOCKET) {
    std::cout << "::socket ffailed. err:" << _sock_err() << std::endl;
    return socket;
  }
  SetSocketOpt(socket);
  return socket;
}

// 创建连接对象
void Network::CreateConnectObj(SOCKET socket) {
  ConnectObj *pConnectObj = new ConnectObj(this, socket);
  if (_connects.find(socket) != _connects.end()) {
    std::cout << "Network::CreateConnectObj. socket is exist. socket:" << socket
              << std::endl;
  }
  _connects[socket] = pConnectObj;
#ifdef EPOLL
  AddEvent(_epfd, socket, EPOLLIN | EPOLLRDHUP);
#endif
}

#ifdef EPOLL

#define RemoveConnectObj(iter)                                                 \
  iter->second->Dispose();                                                     \
  DeleteEvent(_epfd, iter->first);                                             \
  delete iter->second;                                                         \
  iter = _connects.erase(iter);

// 初始化 epoll
void Network::InitEpoll() {
  _epfd = epoll_create(MAX_CLIENT);
  AddEvent(_epfd, _masterSocket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
}

// epoll 处理
void Network::Epoll() {
  _mainSocketEventIndex = -1;
  for (auto iter = _connects.begin(); iter != _connects.end(); iter++) {
    ConnectObj *pObj = iter->second;
    if (pObj->IsClose()) {
      std::cout << "logical layer requires shutdown. socket:" << iter->first
                << std::endl;
      RemoveConnectObj(iter);
      continue;
    }
    if (pObj->HasSendData()) {
      ModifyEvent(_epfd, iter->first, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
    }
  }
  int nfds = epoll_wait(_epfd, _events, MAX_EVENT, 50);
  for (int index = 0; index < nfds; index++) {
    int fd = _events[index].data.fd;
    if (fd == _masterSocket) {
      _mainSocketEventIndex = index;
    }
    auto iter = _connects.find(fd);
    if (iter == _connects.end()) {
      continue;
    }
    if (_events[index].events & EPOLLRDHUP ||
        _events[index].events & EPOLLERR || _events[index].events & EPOLLHUP) {
      RemoveConnectObj(iter);
      continue;
    }
    if (_events[index].events & EPOLLIN) {
      if (!iter->second->Recv()) {
        RemoveConnectObj(iter);
        continue;
      }
    }
    if (_events[index].events & EPOLLOUT) {
      if (!iter->second->Send()) {
        RemoveConnectObj(iter);
        continue;
      }
      ModifyEvent(_epfd, iter->first, EPOLLIN | EPOLLRDHUP);
    }
  }
}

// 添加事件到 epoll
void Network::AddEvent(int epollfd, int fd, int flag) {
  struct epoll_event ev;
  ev.events = flag;
  ev.data.ptr = nullptr;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

// 修改 epoll 中的事件
void Network::ModifyEvent(int epollfd, int fd, int flag) {
  struct epoll_event ev;
  ev.events = flag;
  ev.data.ptr = nullptr;
  ev.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

// 从 epoll 中删除事件
void Network::DeleteEvent(int epollfd, int fd) {
  epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
}
#else
#define RemoveConnectObj(iter)                                                 \
  iter->second->Dispose();                                                     \
  delete iter->second;                                                         \
  iter = _connects.erase(iter);

// 使用 select 处理
void Network::Select() {
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);

  FD_SET(_masterSocket, &readfds);
  FD_SET(_masterSocket, &writefds);
  FD_SET(_masterSocket, &exceptfds);

  SOCKET fdmax = _masterSocket;
  for (auto iter = _connects.begin(); iter != _connects.end(); ++iter) {
    ConnectObj *pObj = iter->second;
    if (pObj->IsClose()) {
      std::cout << "logical layer requires shutdown. socket:" << iter->first
                << std::endl;
      RemoveConnectObj(iter);
      continue;
    }
    if (iter->first > fdmax)
      fdmax = iter->first;
    FD_SET(iter->first, &readfds);
    FD_SET(iter->first, &exceptfds);
    if (iter->second->HasSendData()) {
      FD_SET(iter->first, &writefds);
    }
  }
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 50 * 1000;
  int nfds = ::select(fdmax + 1, &readfds, &writefds, &exceptfds, &timeout);
  if (nfds <= 0)
    return;
  auto iter = _connects.begin();
  while (iter != _connects.end()) {
    if (FD_ISSET(iter->first, &exceptfds)) {
      std::cout << "socket except !! socket:" << iter->first << std::endl;
      RemoveConnectObj(iter);
      continue;
    }
    if (FD_ISSET(iter->first, &readfds)) {
      if (!iter->second->Recv()) {
        RemoveConnectObj(iter);
        continue;
      }
    }
    if (FD_ISSET(iter->first, &writefds)) {
      if (!iter->second->Send()) {
        RemoveConnectObj(iter);
        continue;
      }
    }
    ++iter;
  }
}
#endif

void Network::Update() {
  std::list<Packet *> _tmpSendMsgList;
  _sendMsgMutex.lock();
  std::copy(_sendMsgList.begin(), _sendMsgList.end(),
            std::back_inserter(_tmpSendMsgList));
  _sendMsgList.clear();
  _sendMsgMutex.unlock();

  for (auto pPacket : _tmpSendMsgList) {
    auto iter = _connects.find(pPacket->GetSocket());
    if (iter == _connects.end()) {
      std::cout << "UpdateSendPacket. can't find socket." << std::endl;
      continue;
    }

    iter->second->SendPacket(pPacket);
  }

  _tmpSendMsgList.clear();
}

void Network::HandleDisconnect(Packet *pPacket) {
  auto iter = _connects.find(pPacket->GetSocket());
  if (iter == _connects.end()) {
    std::cout << "dis connect failed. socket not find. socket:"
              << pPacket->GetSocket() << std::endl;
    return;
  }

  iter->second->Close();
}