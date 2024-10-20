#pragma once

#include "base_buffer.h"
#include "common.h"
#include "entity.h"
#include "socket_object.h"
#include <cstring>

#pragma pack(push)
#pragma pack(4)

struct PacketHead {
  unsigned short MsgId;
};

#pragma pack(pop)

#define DEAULT_PACKET_BUFFER_SIZE 1024 * 10

class Packet : public Entity<Packet>,
               public Buffer,
               public NetworkIdentify,
               public IAwakeFromPoolSystem<Proto::MsgId, NetworkIdentify*> {
public:
  Packet();
  ~Packet();

  void Awake(const Proto::MsgId msgId, NetworkIdentify* pIdentify) override;

  template <class ProtoClass> ProtoClass ParseToProto() {
    ProtoClass proto;
    proto.ParsePartialFromArray(GetBuffer(), GetDataLength());
    return proto;
  }

  template <class ProtoClass> void SerializeToBuffer(ProtoClass &protoClass) {
    auto total = (unsigned int)protoClass.ByteSizeLong();
    while (GetEmptySize() < total) {
      ReAllocBuffer();
    }
    protoClass.SerializePartialToArray(GetBuffer(), total);
    FillData(total);
  }

  void SerializeToBuffer(const char* s,unsigned int len){
    while(GetEmptySize() < len){
      ReAllocBuffer();
    }
    ::memcpy(_buffer+_endIndex, s, len);
    FillData(len);
  }

  void BackToPool() override;

  char *GetBuffer() const;
  unsigned short GetDataLength() const;
  int GetMsgId() const;
  void FillData(unsigned int size);
  void ReAllocBuffer();

  // ref
  void AddRef();
  void RemoveRef();
  void OpenRef();
  bool CanBack2Pool();

private:
  Proto::MsgId _msgId;
  
  std::atomic<int> _ref{0};
  bool _isRefOpen{false};
};