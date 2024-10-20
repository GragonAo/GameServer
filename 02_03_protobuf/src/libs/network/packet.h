#pragma once

#include "base_buffer.h"

#pragma pack(push)
#pragma pack(4)

struct PacketHaed {
  unsigned short MsgId;
};
#pragma pack(pop)

#define DEFAULT_PACKET_BUFFER_SIZE 1024 * 10

class Packet : public Buffer {
public:
  Packet();
  Packet(const int msgId);
  ~Packet();

  template <class ProtoClass> ProtoClass ParseToProto() {
    ProtoClass proto;
    proto.ParsePartialFromArray(GetBuffer(), GetDataLength());
    return proto;
  }
  template <class ProtoClass> void SerializeToBuffer(ProtoClass &protoClass) {
    auto total = protoClass.ByteSizeLong();
    while(GetEmptySize() < total){
      ReAllocBuffer();
    }
    protoClass.SerializePartialToArray(GetBuffer(),total);
    FillData(total);
  }
  void Dispose() override;
  void CleanBuffer();
  char *GetBuffer() const;
  unsigned short GetDataLength() const;
  int GetMsgId() const;
  void FillData(unsigned int size);
  void ReAllocBuffer();

private:
  int _msgId;
};