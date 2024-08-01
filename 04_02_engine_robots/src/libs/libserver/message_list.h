#pragma once

#include "disposable.h"
#include "packet.h"
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <mutex>
class MessageCallBackFunctionInfo {
public:
  virtual ~MessageCallBackFunctionInfo() = default;
  virtual bool IsFollowMsgId(Packet *packet) = 0;
  virtual void ProcessPacket() = 0;
  void AddPacket(Packet *pPacket);

protected:
  std::mutex _msgMutex;
  std::list<Packet *> _msgList;
};

class MessageCallBackFunction : public MessageCallBackFunctionInfo {
public:
  using HandleFunction = std::function<void(Packet *)>;
  void RegisterFunction(int msgId, HandleFunction function);
  bool IsFollowMsgId(Packet *packet) override;
  void ProcessPacket() override;
  std::map<int, HandleFunction> &GetCallBackHandler() {
    return _callbackHandle;
  }

protected:
  std::map<int, HandleFunction> _callbackHandle;
};

template <class T>
class MessageCallBackFunctionFilterObj : public MessageCallBackFunction {
public:
  using HandleFunctionWithObj = std::function<void(T *, Packet *)>;
  using HandleGetObject = std::function<T *(SOCKET)>;

  void RegisterFunctionWithObj(int msgId, HandleFunctionWithObj function);
  bool IsFollowMsgId(Packet *packet) override;
  void ProcessPacket() override;

  HandleGetObject GetPacketObject{nullptr};
  void CopyFrom(MessageCallBackFunction *pInfo);

private:
  std::map<int, HandleFunctionWithObj> _callbackHandleWithObj;
};

template <class T>
void MessageCallBackFunctionFilterObj<T>::RegisterFunctionWithObj(
    const int msgId, HandleFunctionWithObj function) {
  _callbackHandleWithObj[msgId] = function;
}

template <class T>
bool MessageCallBackFunctionFilterObj<T>::IsFollowMsgId(Packet *packet) {
  if (MessageCallBackFunction::IsFollowMsgId(packet))
    return true;
  if (_callbackHandleWithObj.find(packet->GetMsgId()) !=
      _callbackHandleWithObj.end()) {
    if (GetPacketObject != nullptr) {
      T *pObj = GetPacketObject(packet->GetSocket());
      if (pObj != nullptr)
        return true;
    }
  }
  return false;
}

template <class T> void MessageCallBackFunctionFilterObj<T>::ProcessPacket() {
  std::list<Packet *> tmpList;
  _msgMutex.lock();
  std::copy(_msgList.begin(), _msgList.end(), std::back_inserter(tmpList));
  _msgList.clear();
  _msgMutex.unlock();
  for (auto packet : tmpList) {
    const auto handleIter = _callbackHandle.find(packet->GetMsgId());
    if (handleIter != _callbackHandle.end()) {
      handleIter->second(packet);
      continue;
    }
    auto iter = _callbackHandleWithObj.find(packet->GetMsgId());
    if (iter != _callbackHandleWithObj.end()) {
      if (GetPacketObject != nullptr) {
        T *pObj = GetPacketObject(packet->GetSocket());
        if (pObj != nullptr) {
          iter->second(pObj, packet);
        }
      }
    }
  }
  tmpList.clear();
}

template <class T>
void MessageCallBackFunctionFilterObj<T>::CopyFrom(
    MessageCallBackFunction *pInfo) {
  auto copyFromData = pInfo->GetCallBackHandler();
  std::transform(copyFromData.begin(), copyFromData.end(),
                 std::back_inserter(_callbackHandle),
                 [](const std::pair<int, HandleFunction> &p) { return p; });
}

// 定义处理函数类型为接收一个 Packet 指针并返回 void
typedef std::function<void(Packet *)> HandleFunction;

class MessageList : public IDisposable {
public:
  void Dispose() override;
  void AttachCallBackHandler(MessageCallBackFunctionInfo *pCallback);

  // 检查消息 ID 是否在处理列表中
  bool IsFollowMsgId(Packet* packet) const;

  // 处理消息队列中的所有包
  void Processpacket() const;

  // 将一个 Packet 添加到消息队列中
  void AddPacket(Packet *pPacket) const;

  // 静态方法：分发 Packet
  static void DisPatchPacket(Packet *pPacket);

  // 静态方法：发送 Packet
  static void SendPacket(Packet *pPacket);

protected:
  MessageCallBackFunctionInfo *_pCallBackFuns{nullptr};
};
