#pragma once

#include "disposable.h"
#include "packet.h"
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <mutex>

class IMessageCallBackFunction {
public:
  virtual ~IMessageCallBackFunction() = default;
  virtual bool IsFollowMsgId(Packet *packet) = 0;
  virtual void ProcessPacket(Packet *packet) = 0;
};

class MessageCallBackFunction : public IMessageCallBackFunction {
public:
  using HandleFunction = std::function<void(Packet *)>;
  void RegisterFunction(int msgId, HandleFunction function);
  bool IsFollowMsgId(Packet *packet) override;
  void ProcessPacket(Packet *packet) override;

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
  using HandleGetObject = std::function<T *(NetworkIdentify*)>;

  void RegisterFunctionWithObj(int msgId, HandleFunctionWithObj function);
  bool IsFollowMsgId(Packet *packet) override;
  void ProcessPacket(Packet *packet) override;

  HandleGetObject GetPacketObject{nullptr};

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
      T *pObj = GetPacketObject(packet);
      if (pObj != nullptr)
        return true;
    }
  }
  return false;
}

template <class T> void MessageCallBackFunctionFilterObj<T>::ProcessPacket(Packet *packet) {
    const auto handleIter = _callbackHandle.find(packet->GetMsgId());
    if(handleIter != _callbackHandle.end()){
      handleIter->second(packet);
      return;
    }

    auto iter = _callbackHandleWithObj.find(packet->GetMsgId());
    if (iter != _callbackHandleWithObj.end()) {
      if (GetPacketObject != nullptr) {
        T *pObj = GetPacketObject(packet);
        if (pObj != nullptr) {
          iter->second(pObj, packet);
        }
      }
    }
}
