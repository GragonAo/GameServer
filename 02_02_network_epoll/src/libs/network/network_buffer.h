#pragma once

#include "base_buffer.h"

#if TestNetwork
#define DEFAULT_SEND_BUFFER_SIZE 10
#define DDEFAULT_RECV_BUFFER_SIZEE 10
#else
// 128KB
#define DEFAULT_SEND_BUFFER_SIZE 1024 * 128
#define DEFAULT_RECV_BUFFER_SIZE 1024 * 128
#endif

class Packet;

using TotalSizeType = unsigned short;

class NetworkBuffer : public Buffer
{
public:
    // explicit 关键字用于构造函数和转换运算符时，能够防止隐式转换和复制初始化，从而提高代码的安全性和可读性。
    explicit NetworkBuffer(const unsigned int size);
    virtual ~NetworkBuffer();
    bool HasData() const;
    unsigned int GetEmptySize() override;
    unsigned int GetWriteSize() const;
    unsigned int GetReadSize() const;

    void FillData(unsigned int size);
    void RemoveData(unsigned int size);
    void ReAllocBuffer();

protected:
    unsigned int _dataSize;
};

class RecvNetworkBuffer : public NetworkBuffer
{
public:
    explicit RecvNetworkBuffer(unsigned int _size);
    void Dispose() override;
    int GetBuffer(char *&pBuffer) const;
    Packet *GetPacket();

private:
    void MemcpyFromBuffer(char *pVoid, unsigned int size);
};

class SendNetworkBuffer : public NetworkBuffer
{
public:
    explicit SendNetworkBuffer(unsigned int _size);
    void Dispose() override;
    int GetBuffer(char *&pBuffer) const;
    void AddPacket(Packet *pPacket);

private:
    void MemcpyToBuffer(char *pVoid, unsigned int size);
};