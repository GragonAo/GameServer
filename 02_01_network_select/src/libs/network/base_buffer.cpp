#include "base_buffer.h"
#include <iostream>
#include <cstring>

// 获取缓冲区中剩余的空闲大小
unsigned int Buffer::GetEmptySize() {
    return _bufferSize - _endIndex;
}

// 重新分配缓冲区大小
void Buffer::ReAllocBuffer(const unsigned int dataLength) {
    if (_bufferSize >= MAX_SIZE) {
        std::cout << "Buffer::Realloc except!! " << std::endl;
        return;
    }

    // 创建新的缓冲区，并增加 ADDITIONAL_SIZE 的大小
    char* tempBuffer = new char[_bufferSize + ADDITIONAL_SIZE];
    unsigned int _newEndIndex;

    // 如果开始索引小于结束索引，直接复制缓冲区内容
    if (_beginIndex < _endIndex) {
        ::memcpy(tempBuffer, _buffer + _beginIndex, _endIndex - _beginIndex);
        _newEndIndex = _endIndex - _beginIndex;
    } else {
        // 如果缓冲区为空且没有要写入的数据，则新结束索引为0
        if (_beginIndex == _endIndex && dataLength <= 0) {
            _newEndIndex = 0;
        } else {
            // 复制从开始索引到缓冲区末尾的数据
            ::memcpy(tempBuffer, _buffer + _beginIndex, _bufferSize - _beginIndex);
            _newEndIndex = _bufferSize - _beginIndex;

            // 如果结束索引大于0，则继续复制缓冲区开头的数据
            if (_endIndex > 0) {
                ::memcpy(tempBuffer + _newEndIndex, _buffer, _endIndex);
                _newEndIndex += _endIndex;
            }
        }
    }

    // 更新缓冲区大小
    _bufferSize += ADDITIONAL_SIZE;
    
    // 释放旧的缓冲区内存
    delete[] _buffer;
    
    // 更新缓冲区指针，开始索引和结束索引
    _buffer = tempBuffer;
    _beginIndex = 0;
    _endIndex = _newEndIndex;
}