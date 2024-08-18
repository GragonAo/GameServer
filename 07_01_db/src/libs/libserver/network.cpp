#include "network.h"
#include "common.h"
#include "connect_obj.h"
#include "object_pool.h"
#include "packet.h"

void Network::BackToPool() { Clean(); }

void Network::Clean() {
  for (auto iter = _connects.begin(); iter != _connects.end(); ++iter) {
    auto pObj = iter->second;
    pObj->ComponentBackToPool();
  }
  _connects.clear();

#ifdef EPOLL
  ::close(_epfd);
#endif

  _sock_close(_masterSocket);
  _masterSocket = INVALID_SOCKET;
}

// 重写 RegisterMsgFunction 方法，注册消息处理函数
void Network::RegisterMsgFunction() {
  auto pMsgCallBack = new MessageCallBackFunction();
  AttachCallbackHandler(pMsgCallBack);
  pMsgCallBack->RegisterFunction(Proto::MsgId::MI_NetworkDisconnectToNet,
                                 BindFunP1(this, &Network::HandleDisconnect));
}

// 发送数据包
void Network::SendPacket(Packet *&pPacket) {
  std::lock_guard<std::mutex> guard(_sendMsgMutex);
  _sendMsgList.GetWriterCache()->emplace_back(pPacket);
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
    std::cout << "::socket failed. err:" << _sock_err() << std::endl;
    return socket;
  }
  SetSocketOpt(socket);
  return socket;
}

// 创建连接对象
void Network::CreateConnectObj(SOCKET socket) {
  ConnectObj *pConnectObj =
      DynamicObjectPool<ConnectObj>::GetInstance()->MallocObject(socket);
  pConnectObj->SetParent(this);
  pConnectObj->SetEntitySystem(GetEntitySystem());
  if (_connects.find(socket) != _connects.end()) {
    std::cout << "Network::CreateConnectObj. socket is exist. socket:" << socket
              << std::endl;
  }
  _connects[socket] = pConnectObj;

#ifdef EPOLL
  AddEvent(_epfd, socket, EPOLLIN | EPOLLET | EPOLLRDHUP);
#endif
}

#ifdef EPOLL

#define RemoveConnectObj(iter)                                                 \
  iter->second->ComponentBackToPool();                                         \
  DeleteEvent(_epfd, iter->first);                                             \
  iter = _connects.erase(iter);

// 初始化 epoll
void Network::InitEpoll() {
  _epfd = epoll_create(MAX_CLIENT);
  AddEvent(_epfd, _masterSocket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
}

// epoll 处理
void Network::Epoll() {
  _mainSocketEventIndex = -1;
  const int nfds = epoll_wait(_epfd, _events, MAX_EVENT, 0);
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
  iter->second->ComponentBackToPool();                                         \
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
    if (iter->first > fdmax)
      fdmax = iter->first;
    FD_SET(iter->first, &readfds);
    FD_SET(iter->first, &exceptfds);
    if (pObj->HasSendData()) {
      FD_SET(iter->first, &writefds);
    }
  }
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
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

void Network::OnNetworkUpdate() {

  _sendMsgMutex.lock();
  if (_sendMsgList.CanSwap()) {
    _sendMsgList.Swap();
  }
  _sendMsgMutex.unlock();

  auto pList = _sendMsgList.GetReaderCache();
  for (auto iter = pList->begin(); iter != pList->end(); iter++) {
    auto pPacket = (*iter);
    auto socket = pPacket->GetSocket();
    auto itConnectObj = _connects.find(socket);
    if (itConnectObj == _connects.end()) {
      std::cout << "send packet. can't find socket:" << socket
                << "msgId:" << pPacket->GetMsgId() << std::endl;
      continue;
    }
    itConnectObj->second->SendPacket(pPacket);
#ifdef EPOLL
    ModifyEvent(_epfd, socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
#endif
  }

  pList->clear();
}

void Network::HandleDisconnect(Packet *pPacket) {
  auto socket = pPacket->GetSocket();
  auto iter = _connects.find(socket);
  if (iter == _connects.end()) {
    std::cout << "dis connect failed. socket not find. socket:"
              << pPacket->GetSocket() << std::endl;
    return;
  }
  RemoveConnectObj(iter);
  std::cout << "logical layer requires shutdown. socket:" << socket
            << std::endl;
}