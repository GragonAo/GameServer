#pragma once

#include "base_buffer.h"

#if TestNetwork
#define DEFAULT_SEND_BUFFER_SIZE 10 // 测试网络环境下的默认发送缓冲区大小
#define DEFAULT_RECV_BUFFER_SIZE 10 // 测试网络环境下的默认接收缓冲区大小
#else
#define DEFAULT_SEND_BUFFER_SIZE 1024 * 128 // 正式网络环境下的默认发送缓冲区大小
#define DEFAULT_RECV_BUFFER_SIZE 1024 * 128 // 正式网络环境下的默认接收缓冲区大小
#endif

class Packet;

using TotalSizeType = unsigned short; // 定义总大小类型

class NetWorkBuffer : public Buffer {
public:
    explicit NetWorkBuffer(const unsigned int size); // 显式构造函数，初始化缓冲区大小
    virtual ~NetWorkBuffer(); // 虚析构函数
    bool HasData() const; // 判断缓冲区是否有数据
    unsigned int GetEmptySize() override; // 获取缓冲区的空余大小
    unsigned int GetWriteSize() const;  //获取可写入数据大小
    unsigned int GetReadSize() const; //获取可读取数据大小

    void FillData(unsigned int size); // 填充缓冲区数据
    void RemoveData(unsigned int size); // 从缓冲区移除数据
    void ReAllocBuffer(); // 重新分配缓冲区大小
protected:
    unsigned int _dataSize; // 获取缓冲区的数据大小
};

class RecvNetworkBuffer : public NetWorkBuffer {
public:
    explicit RecvNetworkBuffer(unsigned int _size); // 显式构造函数，初始化接收缓冲区大小
    void Dispose() override; // 重写释放资源的虚函数
    int GetBuffer(char*& pBuffer) const; // 获取缓冲区指针
    Packet* GetPacket(); // 获取数据包
private:
    void MemcpyFromBuffer(char* pVoid, unsigned int size); // 从缓冲区复制数据
};

class SendNetworkBuffer : public NetWorkBuffer {
public:
    explicit SendNetworkBuffer(unsigned int _size); // 显式构造函数，初始化发送缓冲区大小
    void Dispose() override; // 重写释放资源的虚函数
    int GetBuffer(char*& pBuffer) const; // 获取缓冲区指针
    void AddPacket(Packet* pPacket); // 添加数据包到缓冲区
private:
    void MemcpyToBuffer(char* pVoid, unsigned int size); // 将数据复制到缓冲区
};
