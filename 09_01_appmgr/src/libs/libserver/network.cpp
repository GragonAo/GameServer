#include "network.h"
#include "common.h"
#include "connect_obj.h"
#include "log4_help.h"
#include "network_help.h"
#include "object_pool.h"
#include "object_pool_packet.h"
#include "packet.h"
#include "socket_object.h"
#include <sys/socket.h>
#include "trace_component.h"
#include "component_help.h"

void Network::BackToPool() {
  _sendMsgList.BackToPool();
  for (auto iter = _connects.begin(); iter != _connects.end(); ++iter) {
    auto pObj = iter->second;
    pObj->ComponentBackToPool();
  }
  _connects.clear();

#ifdef EPOLL
  ::close(_epfd);
  _epfd = -1;
#endif
}

// 设置套接字选项
void Network::SetSocketOpt(SOCKET socket) {
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
    LOG_WARN("getsockopt SO_KEEPALIVE failed. err:"
             << _sock_err() << " socket:" << socket
             << " networktype:" << GetNetworkTypeName(_networkType));
  }
  setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, optlen);
  if (getsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, &optlen) <
      0) {
    LOG_WARN("getsockopt TCP_KEEPIDLE failed. err:"
             << _sock_err() << " socket:" << socket
             << " networktype:" << GetNetworkTypeName(_networkType));
  }
  setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, optlen);
  setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, optlen);

  _sock_nonblock(socket);
}

// 创建套接字
SOCKET Network::CreateSocket() {
  _sock_init();
  SOCKET socket;
  if (_networkType == NetworkType::HttpListen ||
      _networkType == NetworkType::HttpConnector)
    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  else
    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (socket == INVALID_SOCKET) {
    std::cout << "::socket failed. err:" << _sock_err() << std::endl;
    return socket;
  }
  SetSocketOpt(socket);
  return socket;
}

bool Network::CheckSocket(SOCKET socket) {
  int err = EBADF;
  socklen_t len = sizeof(err);
  if (::getsockopt(socket, SOL_SOCKET, SO_ERROR, (char *)(&err), &len) == 0) {
    if (!NetworkHelp::IsError(err)) {
      err = 0;
    }
  }
  if (err != 0) {
    _sock_close(socket);
    return false;
  }
  return true;
}

// 创建连接对象
bool Network::CreateConnectObj(SOCKET socket, ObjectKey key,
                               ConnectStateType iState) {
  if (!CheckSocket(socket))
    return false;

  const auto iter = _connects.find(socket);
  if (iter != _connects.end()) {
    LOG_ERROR("Network::CreateConnectObj. socket is exist. socket: "
              << socket << " sn:" << _sn);
    _connects[socket]->ComponentBackToPool();
    _connects.erase(iter);
    return false;
  }

  auto pCollector = GetSystemManager()->GetPoolCollector();
  auto pPool =
      (DynamicObjectPool<ConnectObj> *)pCollector->GetPool<ConnectObj>();
  ConnectObj *pConnectObj = pPool->MallocObject(GetSystemManager(), socket,
                                                _networkType, key, iState);
  pConnectObj->SetParent(this);

  _connects[socket] = pConnectObj;

#ifdef LOG_TRACE_COMPONENT_OPEN
  const auto traceMsg = std::string("create. network type:")
                            .append(GetNetworkTypeName(_networkType));
  ComponentHelp::GetTraceComponent()->Trace(TraceType::Connector, socket,
                                            traceMsg);
#endif

#ifdef EPOLL
  AddEvent(_epfd, socket, EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP);
#endif

  return true;
}

void Network::HandleDisconnect(Packet *pPacket) {
  const auto socketKey = pPacket->GetSocketKey();
  if (socketKey.NetType != _networkType)
    return;
  auto iter = _connects.find(socketKey.Socket);
  if (iter == _connects.end()) {
    std::cout << "dis connect failed. socket not find." << pPacket << std::endl;
    return;
  }

  RemoveConnectObj(iter);
}

#ifdef EPOLL

#define RemoveConnectObj(iter)                                                 \
  iter->second->ComponentBackToPool();                                         \
  DeleteEvent(_epfd, iter->first);                                             \
  iter = _connects.erase(iter);

// 初始化 epoll
void Network::InitEpoll() { _epfd = epoll_create(MAX_CLIENT); }

// epoll 处理
void Network::Epoll() {
  const int nfds = epoll_wait(_epfd, _events, MAX_EVENT, 0);
  for (int index = 0; index < nfds; index++) {
    SOCKET fd = _events[index].data.fd;
    OnEpoll(fd, index);

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

// 使用 select 处理
void Network::Select() {

  for (auto iter = _connects.begin(); iter != _connects.end(); ++iter) {
    if (iter->first > _fdMax)
      _fdMax = iter->first;
    FD_SET(iter->first, &readfds);
    FD_SET(iter->first, &writefds);
    FD_SET(iter->first, &exceptfds);
  }

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  int nfds = ::select(_fdMax + 1, &readfds, &writefds, &exceptfds, &timeout);
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
    auto socketKey = pPacket->GetSocketKey();
    auto iterObj = _connects.find(socketKey.Socket);
    if (iterObj == _connects.end()) {
      LOG_ERROR("failed to send packet. can't find socket." << pPacket);

      DynamicPacketPool::GetInstance()->FreeObject(pPacket);
      continue;
    }
    const auto pObj = iterObj->second;
    if (pObj->GetObjectKey() != pPacket->GetObjectKey()) {
      LOG_ERROR("failed to send packet. connect key is different. packet["
                << pPacket << "] connect:[" << pObj << "]");
      ;
      DynamicPacketPool::GetInstance()->FreeObject(pPacket);
      continue;
    }
    pObj->SendPacket(pPacket);
#ifdef EPOLL
    ModifyEvent(_epfd, socketKey.Socket, EPOLLIN | EPOLLOUT | EPOLLRDHUP);
#endif
  }

  pList->clear();
}

// 发送数据包
void Network::SendPacket(Packet *&pPacket) {
  std::lock_guard<std::mutex> guard(_sendMsgMutex);

  if (pPacket->GetSocketKey().NetType != _networkType) {
    LOG_ERROR("failed to send packet. network type is different." << pPacket);
    return;
  }

  _sendMsgList.GetWriterCache()->emplace_back(pPacket);

#ifdef LOG_TRACE_COMPONENT_OPEN
  const google::protobuf::EnumDescriptor *descriptor =
      Proto::MsgId_descriptor();
  const auto name = descriptor->FindValueByNumber(pPacket->GetMsgId())->name();

  const auto traceMsg = std::string("send net.")
                            .append(" sn:")
                            .append(std::to_string(pPacket->GetSN()))
                            .append(" msgId:")
                            .append(name);
  ComponentHelp::GetTraceComponent()->Trace(
      TraceType::Packet, pPacket->GetSocketKey().Socket, traceMsg);
#endif
}