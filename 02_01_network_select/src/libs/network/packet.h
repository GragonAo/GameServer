#pragma once

#include "base_buffer.h"

#pragma pack(push) // 开始设置结构体成员对齐方式
#pragma pack(4)    // 设置4字节对齐

// 定义数据包头部结构
struct PacketHead
{
    unsigned short MsgId; // 消息ID
};

#pragma pack(pop) // 恢复默认对齐方式

// 根据是否测试网络定义默认缓冲区大小
#if TestNetwork
#define DEFAULT_PACKET_BUFFER_SIZE 10
#else
#define DEFAULT_PACKET_BUFFER_SIZE 1024 * 10
#endif

// Packet 类继承自 Buffer 类
class Packet : public Buffer
{
public:
    Packet();
    Packet(const int msgId);
    ~Packet();

    // 实现 IDisposable 的 Dispose 方法
    void Dispose() override;

    // 清空缓冲区
    void CleanBuffer();

    // 获取缓冲区指针
    char *GetBuffer() const;

    // 添加数据到缓冲区
    void AddBuffer(const char *pBuffer, const unsigned int size);
    
    //获取数据长度
    unsigned short GetDataLength() const;

    // 获取消息ID
    int GetMsgId() const;

    // 填充数据
    void FillData(unsigned int size);

    // 重新分配缓冲区
    void ReAllocBuffer();

private:
    int _msgId; // 消息ID
};