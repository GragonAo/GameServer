#include "packet.h"
#include "network_buffer.h"

#include <cstdlib>
#include <iostream>
#include <cstring>

// NetWorkBuffer

// 构造函数
NetWorkBuffer::NetWorkBuffer(const unsigned size){
    _bufferSize = size;             // 设置缓冲区大小
    _beginIndex = 0;                // 设置读取索引初始值
    _endIndex = 0;                  // 设置写入索引初始值
    _dataSize = 0;                  // 设置数据大小初始值
    _buffer = new  char[_bufferSize];   // 分配缓冲区内存
}

// 析构函数
NetWorkBuffer::~NetWorkBuffer(){
    if(_buffer != nullptr)
        delete[] _buffer;          // 释放缓冲区内存
}

// 检查缓冲区是否有数据
bool NetWorkBuffer::HasData() const{
    if(_dataSize <= 0)              // 数据大小小于等于0表示没有数据
        return false;
    if(_dataSize < sizeof(PacketHead)) // 数据大小小于PacketHead大小则表示数据不完整
        return false;
    return true;
}

// 获取缓冲区空闲大小
unsigned int NetWorkBuffer::GetEmptySize(){
    return _bufferSize - _dataSize; // 空闲大小等于缓冲区大小减去当前数据大小
}

// 获取可写入数据大小
unsigned int NetWorkBuffer::GetWriteSize() const{
    if(_beginIndex <= _endIndex)    // 若写入索引大于等于读取索引
        return _bufferSize - _endIndex; // 可写入大小为缓冲区结尾到写入索引之间的空间大小
    return _beginIndex - _endIndex; // 否则为写入索引到读取索引之间的空间大小
}

// 获取可读取数据大小
unsigned int NetWorkBuffer::GetReadSize() const{
    if(_dataSize <= 0)              // 若数据大小小于等于0，则没有可读取数据
        return 0;
    if(_beginIndex < _endIndex)     // 若读取索引小于写入索引
        return _endIndex - _beginIndex; // 可读取大小为读取索引到写入索引之间的空间大小
    return _bufferSize - _beginIndex; // 否则为读取索引到缓冲区结尾之间的空间大小
}

//填充缓冲区的长度 作用：修改EndIndex的位置。
void NetWorkBuffer::FillDate(unsigned int size){
    _dataSize += size;              // 增加数据大小
    if((_bufferSize - _endIndex) <= size){ // 如果剩余空间不足以容纳size大小的数据
        size -= _bufferSize -_endIndex;    // 计算剩余需要填充的数据大小
        _endIndex = 0;                      // 将写入索引置为缓冲区开头
    }
    _endIndex += size;              // 填充数据后更新写入索引
}

// 移除数据
void NetWorkBuffer::RemoveData(unsigned int size){
    _dataSize -= size;              // 减少数据大小
    if((_beginIndex + size) >= _bufferSize){ // 如果读取索引加上size大小超出了缓冲区
        size -= _bufferSize - _beginIndex;  // 计算剩余需要移除的数据大小
        _beginIndex = 0;                     // 将读取索引置为缓冲区开头
    }
    _beginIndex += size;            // 移除数据后更新读取索引
}

// 重新分配缓冲区大小
void NetWorkBuffer::ReAllocBuffer(){
    Buffer::ReAllocBuffer(_dataSize); // 重新分配缓冲区大小，保留数据
}

// RecvNetworkBuffer

// 构造函数
RecvNetworkBuffer::RecvNetworkBuffer(const unsigned int _size):NetWorkBuffer(_size){

}

// 释放资源
void RecvNetworkBuffer::Dispose(){

}

// 获取缓冲区可写入位置和大小
int RecvNetworkBuffer::GetBuffer(char*& pBuffer) const{
    pBuffer = _buffer + _endIndex;  // 设置可写入位置为写入索引位置
    return GetWriteSize();          // 返回可写入大小
}

// 从缓冲区中获取一个完整的Packet对象
Packet* RecvNetworkBuffer::GetPacket(){
    if(_dataSize < sizeof(TotalSizeType))  // 如果数据大小小于TotalSizeType大小
        return nullptr;                     // 返回空指针，表示没有完整的Packet
    unsigned short totalSize;               // 用于存储总数据大小
    MemcpyFromBuffer(reinterpret_cast<char*>(&totalSize),sizeof(TotalSizeType)); // 从缓冲区中复制总大小数据
    if(_dataSize < totalSize)               // 如果数据大小小于总大小
        return nullptr;                     // 返回空指针，表示数据不完整
    RemoveData(sizeof(TotalSizeType));      // 移除TotalSizeType大小的数据
    PacketHead head;                        // 创建Packet头部对象
    MemcpyFromBuffer(reinterpret_cast<char*>(&head),sizeof(PacketHead)); // 从缓冲区中复制Packet头部数据
    RemoveData(sizeof(PacketHead));         // 移除Packet头部大小的数据
    Packet* pPacket = new Packet(head.MsgId); // 创建新的Packet对象
    const auto dataLength = totalSize - sizeof(PacketHead) - sizeof(TotalSizeType); // 计算数据部分大小
    while (pPacket -> GetTotalSize() < dataLength) // 如果Packet的数据大小小于数据部分大小
    {
        pPacket->ReAllocBuffer();           // 重新分配Packet数据缓冲区大小
    }
    MemcpyFromBuffer(pPacket->GetBuffer(),dataLength); // 从缓冲区中复制数据到Packet中
    pPacket->FillData(dataLength);          // 填充Packet数据
    RemoveData(dataLength);                 // 移除数据部分大小的数据
    return pPacket;                         // 返回Packet对象
}

// 从缓冲区中复制数据到指定的内存位置
void RecvNetworkBuffer::MemcpyFromBuffer(char* pVoid,const unsigned int size){
    const auto readSize = GetReadSize();    // 获取可读取数据大小
    if(readSize < size){                    // 如果可读取大小小于复制大小
        ::memcpy(pVoid,_buffer + _beginIndex,readSize);  // 先复制读取索引到缓冲区结尾之间的数据
        ::memcpy(pVoid + readSize,_buffer,size - readSize); // 再复制缓冲区开头到剩余数据部分
    }else{
        ::memcpy(pVoid,_buffer + _beginIndex,size); // 直接复制读取索引到缓冲区结尾之间的数据
    }
}

// SendNetworkBuffer

// 构造函数
SendNetworkBuffer::SendNetworkBuffer(const unsigned int _size):NetWorkBuffer(_size){

}

// 释放资源
void SendNetworkBuffer::Dispose(){

}

// 获取缓冲区可读取位置和大小
int SendNetworkBuffer::GetBuffer(char*& pBuffer) const{
    if(_dataSize <= 0){                     // 如果没有数据可读取
        pBuffer = nullptr;                  // 设置可读取位置为空指针
        return 0;                           // 返回0，表示没有数据可读取
    }
    if(_beginIndex < _endIndex){            // 如果读取索引小于写入索引
        pBuffer = _buffer + _beginIndex;    // 设置可读取位置为读取索引位置
        return _endIndex - _beginIndex;     // 返回可读取大小为写入索引到读取索引之间的空间大小
    }else{
        pBuffer = _buffer + _beginIndex;    // 设置可读取位置为读取索引位置
        return _bufferSize - _beginIndex;   // 返回可读取大小为读取索引到缓冲区结尾之间的空间大小
    }
}

// 添加一个Packet对象到缓冲区中
void SendNetworkBuffer::AddPacket(Packet* pPacket){
    const auto dataLength = pPacket->GetDataLength(); // 获取Packet数据部分大小
    TotalSizeType totalSize = dataLength + sizeof(PacketHead) + sizeof(TotalSizeType); // 计算总大小

    while (GetEmptySize() < totalSize)     // 如果空闲大小小于总大小
    {
        ReAllocBuffer();                    // 重新分配缓冲区大小
    }

    MemcpyToBuffer(reinterpret_cast<char*>(&totalSize),sizeof(TotalSizeType)); // 将总大小数据复制到缓冲区
    PacketHead head;                        // 创建Packet头部对象
    head.MsgId = pPacket->GetMsgId();       // 设置Packet头部消息ID
    MemcpyToBuffer(reinterpret_cast<char*>(&head),sizeof(PacketHead)); // 将Packet头部数据复制到缓冲区
    MemcpyToBuffer(pPacket->GetBuffer(),pPacket->GetDataLength()); // 将Packet数据部分复制到缓冲区
}

// 从指定的内存位置复制数据到缓冲区
void SendNetworkBuffer::MemcpyToBuffer(char* pVoid,const unsigned int size){
    const auto writeSize = GetWriteSize();   // 获取可写入数据大小
    if(writeSize < size){                    // 如果可写入大小小于复制大小
        ::memcpy(_buffer + _endIndex,pVoid,writeSize); // 先复制缓冲区结尾到可写入数据结尾之间的数据
        ::memcpy(_buffer,pVoid + writeSize,size - writeSize); // 再复制可写入数据开头到剩余数据部分
    }else{
        ::memcpy(_buffer + _endIndex,pVoid,size); // 直接复制缓冲区结尾到可写入数据结尾之间的数据
    }
    FillDate(size);                         // 填充数据后更新数据大小
}
