#pragma once

#include "base_buffer.h"         // 包含 Buffer 类的定义，用于处理字节缓冲区
#include "common.h"              // 通用定义和宏
#include "entity.h"              // Entity 类，用于继承实体功能
#include "system.h"              // 系统相关接口和类定义
#include "socket_object.h"       // Socket 相关定义，用于网络通信

#pragma pack(push)
#pragma pack(4)

// PacketHead 结构体用于描述数据包的基本头部信息，包括消息 ID
struct PacketHead {
    unsigned short MsgId;  // 消息 ID，用于标识不同的消息类型
};

#pragma pack(pop)

#if TestNetwork
#define DEFAULT_PACKET_BUFFER_SIZE  10
#else
// 默认缓冲区大小为 10KB
#define DEFAULT_PACKET_BUFFER_SIZE  1024 * 10
#endif

// Packet 类继承自 Entity, Buffer, NetworkIdentify，并实现了 IAwakeFromPoolSystem 接口。
// 用于处理消息的封包与解包，以及消息的网络标识与回收。
class Packet : public Entity<Packet>, public Buffer, public NetworkIdentify, public IAwakeFromPoolSystem<Proto::MsgId, NetworkIdentify*>
{
public:
    // 构造函数
    Packet();
    // 析构函数
    ~Packet();
    
    // 实现 IAwakeFromPoolSystem 的唤醒方法，用于初始化消息 ID 和网络标识
    void Awake(Proto::MsgId msgId, NetworkIdentify* pIdentify) override;

    // 将缓冲区的数据解析为指定的 Proto 类对象
    template<class ProtoClass>
    ProtoClass ParseToProto()
    {
        ProtoClass proto;
        proto.ParsePartialFromArray(GetBuffer(), GetDataLength());  // 从缓冲区解析 Proto 数据
        return proto;
    }

    // 将 Proto 对象序列化为缓冲区数据
    template<class ProtoClass>
    void SerializeToBuffer(ProtoClass& protoClase)
    {
        auto total = (unsigned int)protoClase.ByteSizeLong();  // 计算序列化后的字节长度
        while (GetEmptySize() < total)  // 检查缓冲区是否有足够的空闲空间
        {
            ReAllocBuffer();  // 如果空间不足则重新分配缓冲区
        }

        protoClase.SerializePartialToArray(GetBuffer(), total);  // 将 Proto 数据写入缓冲区
        FillData(total);  // 更新缓冲区使用的大小
    }

    // 将原始字节数据序列化到缓冲区
    void SerializeToBuffer(const char* s, unsigned int len)
    {
        while (GetEmptySize() < len)
        {
            ReAllocBuffer();  // 缓冲区空间不足时重新分配
        }

        ::memcpy(_buffer + _endIndex, s, len);  // 将字节数据复制到缓冲区
        FillData(len);  // 更新缓冲区使用的大小
    }

    // 将 Packet 对象返回到对象池中以供重用
    void BackToPool() override;

    // 获取缓冲区指针
    char* GetBuffer() const;
    // 获取数据长度
    unsigned short GetDataLength() const;
    // 获取消息 ID
    int GetMsgId() const;
    // 填充缓冲区中的数据
    void FillData(unsigned int size);
    // 重新分配缓冲区空间
    void ReAllocBuffer();

    // 引用计数管理
    void AddRef();  // 增加引用计数
    void RemoveRef();  // 减少引用计数
    void OpenRef();  // 打开引用计数机制
    bool CanBack2Pool();  // 判断是否可以返回对象池

private:
    Proto::MsgId _msgId;  // 消息 ID，用于标识数据包的类型

    // 私有变量
    std::atomic<int> _ref{ 0 };  // 引用计数，用于控制对象的回收
    bool _isRefOpen{ false };  // 引用计数机制是否开启
};
