#pragma once 
#include "base_buffer.h"
#include "packet.h"

#define DEFAULT_SEND_BUFFER_SIZE 1024*128
#define DEFAULT_RECV_BUFFER_SIZE 1024*128

class Packet;

using TotalSizeType = unsigned short;

class NetworkBuffer : public Buffer{
    public :
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

class RecvNetworkBuffer : public NetworkBuffer{
    public:
        explicit RecvNetworkBuffer(unsigned int _size);
        void Dispose() override;
        int GetBuffer(char*& pBuffer)const;
        Packet *GetPacket();
    private:
        void MemcpyFromBuffer(char *pVoid,unsigned int size);
};

class SendNetworkBuffer : public NetworkBuffer{
    public:
        explicit SendNetworkBuffer(unsigned int _size);
        void Dispose() override;
        int GetBuffer(char*& pBuffer)const;
        void AddPacket(Packet *pPacket);
    private:
        void MemcpyFromBuffer(char *pVoid,unsigned int size);
};