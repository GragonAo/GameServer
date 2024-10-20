#pragma once

#include "entity.h"
#include "network_type.h"
#include "socket_object.h"
#include "system.h"
#include "time.h" // 包含时间相关的定义
#include <memory> // 用于智能指针的管理

// 前置声明类，表示这些类在其他地方定义
class RecvNetworkBuffer;
class SendNetworkBuffer;
class Packet;

// 定义常量，用于控制心跳包时间和延迟时间
#define PingTime 1000           // 心跳包发送时间间隔
#define PingDelayTime 10 * 1000 // 心跳包延迟时间

//连接对象的状态
enum class ConnectStateType
{
    None,
    Connecting,
    Connected,
};

class ConnectObj : public Entity<ConnectObj>,
                   public NetworkIdentify,
                   public IAwakeFromPoolSystem<SOCKET, NetworkType, ObjectKey,
                                               ConnectStateType> {
public:
  // 构造函数，接受一个 Network 对象指针和一个 socket 描述符
  ConnectObj();

  // 析构函数，销毁对象时调用
  virtual ~ConnectObj();

  void Awake(SOCKET socket, NetworkType networkType, ObjectKey key,
             ConnectStateType state) override;

  virtual void BackToPool() override;

  // 检查是否有接收的数据
  bool HasRecvData() const;
  // 执行接收数据操作
  bool Recv();

  // 发送数据包
  void SendPacket(Packet *pPacket) const;
  // 检查是否有待发送的数据
  bool HasSendData() const;
  // 执行发送数据操作
  bool Send();

  // 关闭连接
  void Close();

  ConnectStateType GetState() const;
  void ChangeStateToConnected();
  void ModifyConnectKey(ObjectKey key);

protected:
  ConnectStateType _state{ConnectStateType::None};
  RecvNetworkBuffer *_recvBuffer{nullptr}; // 接收缓冲区
  SendNetworkBuffer *_sendBuffer{nullptr}; // 发送缓冲区
};
