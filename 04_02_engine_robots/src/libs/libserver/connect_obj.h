#pragma once

#include <memory> // 用于智能指针的管理

#include "disposable.h" // 包含 IDisposable 接口的定义
#include "network.h" // 包含 Network 类的定义
#include "time.h" // 包含时间相关的定义

// 前置声明类，表示这些类在其他地方定义
class RecvNetworkBuffer;
class SendNetworkBuffer;
class Packet;

// 定义常量，用于控制心跳包时间和延迟时间
#define PingTime 1000 // 心跳包发送时间间隔
#define PingDelayTime 10 * 1000 // 心跳包延迟时间

// ConnectObj 类，表示一个网络连接对象，继承自 IDisposable 接口
class ConnectObj : public IDisposable {
public:
  // 构造函数，接受一个 Network 对象指针和一个 socket 描述符
  ConnectObj(Network *pNetwork, SOCKET socket);

  // 析构函数，销毁对象时调用
  ~ConnectObj() override;

  // 实现 IDisposable 接口的 Dispose 方法，用于释放资源
  void Dispose() override;

  // 关闭连接
  void Close();

  // 检查连接是否已关闭
  bool IsClose() const;

  // 获取 socket 描述符
  SOCKET GetSocket() const { return _socket; }

  // 检查是否有接收的数据
  bool HasRecvData() const;

  // 获取接收的数据包
  Packet *GetRecvPacket() const;

  // 执行接收数据操作
  bool Recv() const;

  // 检查是否有待发送的数据
  bool HasSendData() const;

  // 发送数据包
  void SendPacket(Packet *pPacket) const;

  // 执行发送数据操作
  bool Send() const;

protected:
  Network *_pNetwork{nullptr}; // 指向所属的 Network 对象
  const SOCKET _socket; // 保存 socket 描述符
  RecvNetworkBuffer *_recvBuffer{nullptr}; // 接收缓冲区
  SendNetworkBuffer *_sendBuffer{nullptr}; // 发送缓冲区
  bool _isClose{false}; // 标记连接是否关闭
};
