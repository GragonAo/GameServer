#include "thread_collector_exclusive.h"
#include "message_system.h"
#include "packet.h"

ThreadCollectorExclusive::ThreadCollectorExclusive(ThreadType threadType,
                                                   int initNum)
    : ThreadCollector(threadType, initNum) {}

void ThreadCollectorExclusive::HandleMessage(Packet *pPacket) {
  if (pPacket->GetMsgId() == Proto::MsgId::MI_CmdThread) {
    HandlerCreateMessage(pPacket,true);
  } else {
    auto objs = _threads.GetReaderCache();
    auto iter = objs->find(_nextThreadSn);
    if (iter == objs->end()) {
      iter = objs->begin();
    }

    iter->second->GetMessageSystem()->AddPacketToList(pPacket);
    iter++;
    if (iter == objs->end()) {
      iter = objs->begin();
    }
    _nextThreadSn = iter->first;
  }
}
