#include "packet.h"
#include "log4_help.h"

// Packet 类的构造函数，初始化缓冲区大小和索引
Packet::Packet()
{
    _bufferSize = DEFAULT_PACKET_BUFFER_SIZE;  // 初始化默认缓冲区大小
    _beginIndex = 0;  // 数据起始索引
    _endIndex = 0;    // 数据结束索引
    _buffer = new char[_bufferSize];  // 动态分配缓冲区

    _ref = 0;  // 初始化引用计数
    _isRefOpen = false;  // 初始化引用计数机制关闭状态
    _socketKey.Clean();  // 清理网络标识符
    _objKey.Clean();     // 清理对象标识符
}

// Packet 类的析构函数，释放缓冲区内存
Packet::~Packet()
{
    delete[] _buffer;  // 释放缓冲区内存
}

// Awake 函数用于初始化 Packet 对象，用于从对象池唤醒，初始化消息 ID 和网络标识
void Packet::Awake(Proto::MsgId msgId, NetworkIdentify* pIdentify)
{
    if (pIdentify != nullptr)
    {
        _socketKey = pIdentify->GetSocketKey();  // 从 pIdentify 中获取 SocketKey
        _objKey = pIdentify->GetObjectKey();     // 从 pIdentify 中获取 ObjectKey
    }
    else
    {
        _socketKey.Clean();  // 如果未提供识别信息，清理 socket 和对象标识
        _objKey.Clean();
    }

    _msgId = msgId;  // 设置消息 ID
    _beginIndex = 0;  // 重置缓冲区的开始索引
    _endIndex = 0;    // 重置缓冲区的结束索引
    _ref = 0;         // 重置引用计数
    _isRefOpen = false;  // 重置引用计数机制状态
}

// BackToPool 函数用于将 Packet 对象返回到对象池中，重置状态以供下次使用
void Packet::BackToPool()
{
    _msgId = Proto::MsgId::None;  // 重置消息 ID
    _socketKey.Clean();  // 清理 SocketKey
    _objKey.Clean();     // 清理 ObjectKey

    _beginIndex = 0;  // 重置缓冲区的开始索引
    _endIndex = 0;    // 重置缓冲区的结束索引
    _ref = 0;         // 重置引用计数
    _isRefOpen = false;  // 重置引用计数机制状态
}

// 返回缓冲区的指针
char* Packet::GetBuffer() const
{
    return _buffer;
}

// 返回当前缓冲区中的数据长度
unsigned short Packet::GetDataLength() const
{
    return _endIndex - _beginIndex;  // 计算数据长度
}

// 获取消息 ID
int Packet::GetMsgId() const
{
    return _msgId;
}

// 向缓冲区中填充指定大小的数据
void Packet::FillData(const unsigned int size)
{
    _endIndex += size;  // 调整结束索引以反映新数据的加入
}

// 重新分配缓冲区的大小，当现有缓冲区不够大时调用
void Packet::ReAllocBuffer()
{
    Buffer::ReAllocBuffer(_endIndex - _beginIndex);  // 调用基类的方法重新分配缓冲区
}

// 增加引用计数，表示 Packet 对象正在被使用
void Packet::AddRef()
{
    ++_ref;  // 增加引用计数
}

// 减少引用计数，并检查引用计数是否小于 0
void Packet::RemoveRef()
{
    --_ref;  // 减少引用计数
    if (_ref < 0)  // 如果引用计数小于 0，则记录错误日志
    {
        const google::protobuf::EnumDescriptor* descriptor = Proto::MsgId_descriptor();  // 获取枚举描述符
        const auto name = descriptor->FindValueByNumber(_msgId)->name();  // 根据消息 ID 获取对应的名称
        LOG_ERROR("packet ref < 0. ref:" << _ref << " msgId:" << name.c_str());  // 记录错误日志
    }
}

// 打开引用计数机制，允许 Packet 对象被引用
void Packet::OpenRef()
{
    _isRefOpen = true;  // 启用引用计数机制
}

// 检查 Packet 对象是否可以返回对象池，只有当引用计数为 0 且机制开启时可以返回
bool Packet::CanBack2Pool()
{
    if (!_isRefOpen)  // 如果引用计数机制未开启，不能返回对象池
        return false;

    return _ref == 0;  // 如果引用计数为 0，允许返回对象池
}
