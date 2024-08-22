#pragma once

#include "base_buffer.h"
#include "common.h"
#include "entity.h"

#pragma pack(push)
#pragma pack(4)

struct PacketHead {
  unsigned short MsgId;
};
#pragma pack(pop)

#define DEAULT_PACKET_BUFFER_SIZE 1024 * 10

class Packet : public Entity<Packet>,  public Buffer,public IAwakeFromPoolSystem<Proto::MsgId, SOCKET> {
public:
  Packet();
  ~Packet();
  void Awake(const Proto::MsgId msgId, SOCKET socket) override;
  template <class ProtoClass> ProtoClass ParseToProto() {
    ProtoClass proto;
    proto.ParsePartialFromArray(GetBuffer(), GetDataLength());
    return proto;
  }
  template <class ProtoClass> void SerializeToBuffer(ProtoClass &protoClass) {
    auto total = protoClass.ByteSizeLong();
    while (GetEmptySize() < total) {
      ReAllocBuffer();
    }
    protoClass.SerializePartialToArray(GetBuffer(), total);
    FillData(total);
  }
  void BackToPool() override;
  void CleanBuffer();
  
  char *GetBuffer() const;
  unsigned short GetDataLength() const;
  int GetMsgId() const;
  void FillData(unsigned int size);
  void ReAllocBuffer();
  SOCKET GetSocket() const;
  void SetSocket(SOCKET socket);

private:
  Proto::MsgId _msgId;
  SOCKET _socket;
};