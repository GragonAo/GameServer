#pragma once

#include <functional>
#include <list>
#include <map>
#include <mutex>

class Packet;

// 定义处理函数类型为接收一个 Packet 指针并返回 void
typedef std::function<void(Packet *)> HandleFunction;

class MessageList {
public:
  // 注册一个消息 ID 对应的处理函数
  void RegisterFunction(int msgId, HandleFunction function);

  // 检查消息 ID 是否在处理列表中
  bool IsFollowMsgId(int msgId);

  // 处理消息队列中的所有包
  void Processpacket();

  // 将一个 Packet 添加到消息队列中
  void AddPacket(Packet *pPacket);

  // 静态方法：分发 Packet
  static void DisPatchPacket(Packet *pPacket);

  // 静态方法：发送 Packet
  static void SendPacket(Packet *pPacket);

protected:
  // 互斥锁，用于保护对消息列表的访问
  std::mutex _msgMutex;

  // 消息列表，存储待处理的 Packet
  std::list<Packet *> _msgList;

  // 消息 ID 到处理函数的映射
  std::map<int, HandleFunction> _callbackHandle;
};
