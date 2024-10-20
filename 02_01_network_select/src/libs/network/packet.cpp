#include "packet.h"
#include <iostream>
#include <cstring>

// 默认构造函数
Packet::Packet() {
    _msgId = 0; // 初始化消息ID为0
    CleanBuffer(); // 清空缓冲区
    _bufferSize = DEFAULT_PACKET_BUFFER_SIZE; // 设置默认缓冲区大小
    _beginIndex = 0; // 初始化起始索引
    _endIndex = 0; // 初始化结束索引
    _buffer = new char[_bufferSize]; // 分配缓冲区内存
}

// 带消息ID参数的构造函数
Packet::Packet(const int msgId) {
    _msgId = msgId; // 初始化消息ID为传入的值
    CleanBuffer(); // 清空缓冲区
    _bufferSize = DEFAULT_PACKET_BUFFER_SIZE; // 设置默认缓冲区大小
    _beginIndex = 0; // 初始化起始索引
    _endIndex = 0; // 初始化结束索引
    _buffer = new char[_bufferSize]; // 分配缓冲区内存
}

// 析构函数
Packet::~Packet() {
    CleanBuffer(); // 清空缓冲区并释放内存
}

// 实现 IDisposable 的 Dispose 方法
void Packet::Dispose() {
    _msgId = 0; // 重置消息ID
    _beginIndex = 0; // 重置起始索引
    _endIndex = 0; // 重置结束索引
    CleanBuffer(); // 清空缓冲区并释放内存
}

// 清空缓冲区，释放内存
void Packet::CleanBuffer() {
    if (_buffer != nullptr) {
        delete[] _buffer; // 释放缓冲区内存
        _buffer = nullptr; // 将缓冲区指针置空
    }
    _beginIndex = 0; // 重置起始索引
    _endIndex = 0; // 重置结束索引
    _bufferSize = 0; // 重置缓冲区大小
}

// 获取缓冲区指针
char* Packet::GetBuffer() const {
    return _buffer;
}

// 向缓冲区添加数据
void Packet::AddBuffer(const char* pBuffer, const unsigned int size) {
    // 如果剩余空间不足，重新分配缓冲区
    while (GetEmptySize() < size) {
        ReAllocBuffer();
    }
    // 复制数据到缓冲区
    ::memcpy(_buffer + _endIndex, pBuffer, size);
    FillData(size); // 填充数据
}

// 获取数据长度
unsigned short Packet::GetDataLength() const {
    return _endIndex - _beginIndex;
}

// 获取消息ID
int Packet::GetMsgId() const {
    return _msgId;
}

// 填充数据
void Packet::FillData(const unsigned int size) {
    _endIndex += size; // 更新结束索引
}

// 重新分配缓冲区
void Packet::ReAllocBuffer() {
    Buffer::ReAllocBuffer(_endIndex - _beginIndex); // 调用基类方法重新分配缓冲区
}