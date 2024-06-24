#pragma once // 防止头文件被多次包含
#include "disposable.h" // 引入 IDisposable 基类的头文件

// 定义一些常量
#define ADDITIONAL_SIZE 10 // 增量大小，通常用于重新分配缓冲区时的增加量
#define MAX_SIZE 1024*1025 // 缓冲区的最大大小（1MB）

// Buffer 类继承自 IDisposable 类
class Buffer : public IDisposable {
public:
    // 获取缓冲区中剩余的空闲大小
    virtual unsigned int GetEmptySize();

    // 重新分配缓冲区大小
    void ReAllocBuffer(unsigned int dataLength);

    // 获取缓冲区的结束索引
    unsigned int GetEndIndex() const {
        return _endIndex;
    }

    // 获取缓冲区的开始索引
    unsigned int GetBeginIndex() const {
        return _beginIndex;
    }

    // 获取缓冲区的总大小
    unsigned int GetTotalSize() const {
        return _bufferSize;
    }

protected:
    char* _buffer{nullptr}; // 缓冲区指针，初始化为空指针
    unsigned int _beginIndex{0}; // 缓冲区的开始索引，初始化为0
    unsigned int _endIndex{0}; // 缓冲区的结束索引，初始化为0
    unsigned int _bufferSize{0}; // 缓冲区的总大小，初始化为0
};