#pragma once

#include "cache_swap.h"
#include "common.h" // 包含项目中的常用定义或声明
#include "entity.h"
#include "message_system.h"
#include "network_interface.h"
#include "socket_object.h" // 包含 ISocketObject 接口的声明
#include <map>             // 使用 STL 的 map 容器

#include <errno.h>  // 错误号相关的定义
#include <fcntl.h>  // 文件控制定义
#include <stdio.h>  // 标准输入输出库
#include <stdlib.h> // 标准库
#include <string.h> // 字符串操作库
#include <unistd.h> // UNIX 标准库

#include <arpa/inet.h>   // 提供 IP 地址转换函数
#include <netinet/in.h>  // Internet 地址族定义
#include <netinet/tcp.h> // TCP 协议定义
#include <sys/socket.h>  // 套接字库
#include <sys/types.h>   // 基本系统数据类型

#ifdef EPOLL
#include <sys/epoll.h> // epoll 库
#endif

// 套接字初始化宏
#define _sock_init()

// 设置套接字为非阻塞模式的宏
#define _sock_nonblock(sockfd)                                                 \
  {                                                                            \
    int flags = fcntl(sockfd, F_GETFL, 0);                                     \
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);                                \
  }
// 套接字退出宏
#define _sock_exit()

// 获取套接字错误号的宏
#define _sock_err() errno

// 关闭套接字的宏
#define _sock_close(sockfd) ::shutdown(sockfd, SHUT_RDWR)

// 判断套接字是否被阻塞的宏
#define _sock_is_blocked() (errno == EAGAIN || errno == 0)

// 前置声明类
class ConnectObj;
class Packet;

// Network 类，继承自 ThreadObject 和 ISocketObject
class Network : public Entity<Network>, public IMessageSystem, public INetwork {
public:
  void BackToPool() override;

  // 重写 RegisterMsgFunction 方法，注册消息处理函数
  void RegisterMsgFunction() override;

  // 获取套接字
  SOCKET GetSocket() override { return _masterSocket; }

  // 发送数据包
  void SendPacket(Packet *&pPacket) override;

  bool IsBroadcast() { return _isBroadcast; }

protected:
  // 设置套接字选项
  static void SetSocketOpt(SOCKET socket);

  // 创建套接字
  static SOCKET CreateSocket();

  // 创建连接对象
  void CreateConnectObj(SOCKET socket);

  void Clean();

#ifdef EPOLL
  // 初始化 epoll
  void InitEpoll();

  // epoll 处理
  void Epoll();

  // 添加事件到 epoll
  void AddEvent(int epollfd, int fd, int flag);

  // 修改 epoll 中的事件
  void ModifyEvent(int epollfd, int fd, int flag);

  // 从 epoll 中删除事件
  void DeleteEvent(int epollfd, int fd);
#else
  // 使用 select 处理
  void Select();
#endif
  void OnNetworkUpdate();

private:
  // 处理断开连接
  void HandleDisconnect(Packet *pPacket);

protected:
  SOCKET _masterSocket{INVALID_SOCKET};     // 主套接字
  std::map<SOCKET, ConnectObj *> _connects; // 存储连接对象的映射

#ifdef EPOLL
#define MAX_CLIENT 5120                  // 最大客户端数量
#define MAX_EVENT 5120                   // 最大事件数量
  struct epoll_event _events[MAX_EVENT]; // epoll 事件数组
  int _epfd;                             // epoll 文件描述符
  int _mainSocketEventIndex{-1};         // 主套接字事件索引
#else
  fd_set readfds, writefds, exceptfds; // 文件描述符集合
#endif

  std::mutex _sendMsgMutex;       // 发送消息的互斥锁
  CacheSwap<Packet> _sendMsgList; // 发送消息列表
  bool _isBroadcast{true};
};
