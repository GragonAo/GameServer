#include "network_buffer.h"
#include "packet.h"
#include <cstring>

NetworkBuffer::NetworkBuffer(const unsigned int size) {
  _bufferSize = size;
  _beginIndex = 0;
  _endIndex = 0;
  _dataSize = 0;
  _buffer = new char[_bufferSize];
}
NetworkBuffer::~NetworkBuffer() {
  if (_buffer != nullptr)
    delete[] _buffer;
}
bool NetworkBuffer::HasData() const {
  if (_dataSize <= 0)
    return false;
  if (_dataSize < sizeof(PacketHaed))
    return false;
  return true;
}
unsigned int NetworkBuffer::GetEmptySize() { return _bufferSize - _dataSize; }
unsigned int NetworkBuffer::GetWriteSize() const {
  if (_beginIndex <= _endIndex) {
    return _bufferSize - _endIndex;
  } else {
    return _beginIndex - _endIndex;
  }
}
unsigned int NetworkBuffer::GetReadSize() const {
  if (_dataSize <= 0)
    return 0;
  if (_beginIndex < _endIndex)
    return _endIndex - _beginIndex;
  else
    return _bufferSize - _beginIndex;
}
void NetworkBuffer::FillData(unsigned int size) {
  _dataSize += size;
  if ((_bufferSize - _endIndex) <= size) {
    size -= _bufferSize - _endIndex;
    _endIndex = 0;
  }
  _endIndex += size;
}
void NetworkBuffer::RemoveData(unsigned int size) {
  _dataSize -= size;
  if ((_beginIndex + size) >= _bufferSize) {
    size -= _bufferSize - _beginIndex;
    _beginIndex = 0;
  }
  _beginIndex += size;
}
void NetworkBuffer::ReAllocBuffer() { Buffer::ReAllocBuffer(_dataSize); }

//////////////////////////////////////////////////////////////////
RecvNetworkBuffer::RecvNetworkBuffer(const unsigned int _size)
    : NetworkBuffer(_size) {}
void RecvNetworkBuffer::RecvNetworkBuffer::Dispose() {}
int RecvNetworkBuffer::GetBuffer(char *&pBuffer) const {
  pBuffer = _buffer + _endIndex;
  return GetWriteSize();
}
Packet *RecvNetworkBuffer::GetPacket() {
  if (_dataSize < sizeof(TotalSizeType))
    return nullptr;
  unsigned short totalSize;
  MemcpyFromBuffer(reinterpret_cast<char *>(&totalSize), sizeof(TotalSizeType));
  if (_dataSize < totalSize)
    return nullptr;
  RemoveData(sizeof(TotalSizeType));
  PacketHaed head;
  MemcpyFromBuffer(reinterpret_cast<char *>(&head), sizeof(PacketHaed));
  RemoveData(sizeof(PacketHaed));
  Packet *pPacket = new Packet(head.MsgId);
  const auto dataLength = totalSize - sizeof(PacketHaed) - sizeof(TotalSizeType);
  while (pPacket->GetTotalSize() < dataLength) {
    pPacket->ReAllocBuffer();
  }
  MemcpyFromBuffer(pPacket->GetBuffer(), dataLength);
  pPacket->FillData(dataLength);
  RemoveData(dataLength);
  return pPacket;
}
void RecvNetworkBuffer::MemcpyFromBuffer(char *pVoid, unsigned int size) {
  const auto readSize = GetReadSize();
  if (readSize < size) {
    ::memcpy(pVoid, _buffer + _beginIndex, readSize);
    ::memcpy(pVoid + readSize, _buffer, size - readSize);
  } else {
    ::memcpy(pVoid, _buffer + _beginIndex, size);
  }
}

SendNetworkBuffer::SendNetworkBuffer(unsigned int _size)
    : NetworkBuffer(_size) {}
void SendNetworkBuffer::Dispose() {}
int SendNetworkBuffer::GetBuffer(char *&pBuffer) const {
  if (_dataSize <= 0) {
    pBuffer = nullptr;
    return 0;
  }
  if (_beginIndex < _endIndex) {
    pBuffer = _buffer + _beginIndex;
    return _endIndex - _beginIndex;
  } else {
    pBuffer = _buffer + _beginIndex;
    return _bufferSize - _beginIndex;
  }
}
void SendNetworkBuffer::AddPacket(Packet *pPacket) {
  const auto dataLength = pPacket->GetDataLength();
  TotalSizeType totalSize =
      dataLength + sizeof(PacketHaed) + sizeof(TotalSizeType);
  while (GetEmptySize() < totalSize) {
    ReAllocBuffer();
  }
  MemcpyFromBuffer(reinterpret_cast<char *>(&totalSize), sizeof(TotalSizeType));
  PacketHaed head;
  head.MsgId = pPacket->GetMsgId();
  MemcpyFromBuffer(reinterpret_cast<char *>(&head), sizeof(PacketHaed));
  MemcpyFromBuffer(pPacket->GetBuffer(), pPacket->GetDataLength());
}
void SendNetworkBuffer::MemcpyFromBuffer(char *pVoid, unsigned int size) {
  const auto writeSize = GetWriteSize();
  if (writeSize < size) {
    ::memcpy(_buffer + _endIndex, pVoid, writeSize);
    ::memcpy(_buffer, pVoid + writeSize, size - writeSize);
  } else {
    ::memcpy(_buffer + _endIndex, pVoid, size);
  }
  FillData(size);
}