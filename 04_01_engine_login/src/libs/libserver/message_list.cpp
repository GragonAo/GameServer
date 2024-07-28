#include "message_list.h"
#include "packet.h"
#include "thread_mgr.h"
#include <iterator>

// 注册一个消息 ID 对应的处理函数
void MessageList::RegisterFunction(int msgId, HandleFunction function) {
  std::lock_guard<std::mutex> guard(_msgMutex);
  _callbackHandle[msgId] = function;
}

// 检查消息 ID 是否在处理列表中
bool MessageList::IsFollowMsgId(int msgId) {
  std::lock_guard<std::mutex> guard(_msgMutex);
  return _callbackHandle.find(msgId) != _callbackHandle.end();
}

// 处理消息队列中的所有包
void MessageList::Processpacket() {
  std::list<Packet*> tmpList;
  _msgMutex.lock();
  std::copy(_msgList.begin(), _msgList.end(), std::back_inserter(tmpList));
  _msgList.clear();
  _msgMutex.unlock();

  for (auto packet : tmpList) {
    const auto handleIter = _callbackHandle.find(packet->GetMsgId());
    if (handleIter == _callbackHandle.end()) {
      std::cout << "packet is not hander. msg id;" << packet->GetMsgId()
                << std::endl;
    } else {
      handleIter->second(packet);
    }
  }
  tmpList.clear();
}

// 将一个 Packet 添加到消息队列中
void MessageList::AddPacket(Packet *pPacket) {
  std::lock_guard<std::mutex> guard(_msgMutex);
  _msgList.push_back(pPacket);
}

// 静态方法：分发 Packet
void MessageList::DisPatchPacket(Packet *pPacket) {
  ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}

// 静态方法：发送 Packet
void MessageList::SendPacket(Packet *pPacket) {
  ThreadMgr::GetInstance()->SendPacket(pPacket);
}