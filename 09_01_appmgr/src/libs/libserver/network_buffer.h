#pragma once

#include "base_buffer.h"    // 包含 Buffer 基类的定义

// 默认的发送和接收缓冲区大小
#define DEFAULT_SEND_BUFFER_SIZE 1024 * 128
#define DEFAULT_RECV_BUFFER_SIZE 1024 * 128

// 前置声明类
class Packet;
class ConnectObj;

// 使用 typedef 定义总大小类型为无符号短整型
using TotalSizeType = unsigned short;

// NetworkBuffer 类，继承自 Buffer 基类
class NetworkBuffer : public Buffer {
public:
  // 构造函数，接受缓冲区大小和连接对象指针
  NetworkBuffer(const unsigned int size, ConnectObj *pConnect);

  // 虚析构函数
  virtual ~NetworkBuffer();

  void BackToPool();

  // 检查是否有数据
  bool HasData() const;

  // 获取空闲空间大小
  unsigned int GetEmptySize() override;

  // 获取写入数据的大小
  unsigned int GetWriteSize() const;

  // 获取读取数据的大小
  unsigned int GetReadSize() const;

  // 填充数据
  void FillData(unsigned int size);

  // 移除数据
  void RemoveData(unsigned int size);

  // 重新分配缓冲区
  void ReAllocBuffer();

protected:
  unsigned int _dataSize;       // 当前数据大小
  ConnectObj *_pConnectObj{nullptr}; // 关联的连接对象指针
};

// RecvNetworkBuffer 类，继承自 NetworkBuffer
class RecvNetworkBuffer : public NetworkBuffer {
public:
  // 构造函数，接受缓冲区大小和连接对象指针
  RecvNetworkBuffer(unsigned int _size, ConnectObj *pConnectObj);

  // 获取缓冲区数据
  int GetBuffer(char *&pBuffer) const;

  // 获取数据包
  Packet *GetPacket();

protected:
  Packet* GetTcpPacket();
  Packet* GetHttpPacket();

private:
  // 从缓冲区中复制数据
  void MemcpyFromBuffer(char *pVoid, unsigned int size);
};

// SendNetworkBuffer 类，继承自 NetworkBuffer
class SendNetworkBuffer : public NetworkBuffer {
public:
  // 构造函数，接受缓冲区大小和连接对象指针
  SendNetworkBuffer(unsigned int _size, ConnectObj *pConnectObj);

  // 获取缓冲区数据
  int GetBuffer(char *&pBuffer) const;

  // 添加数据包到缓冲区
  void AddPacket(Packet *pPacket);

private:
  // 向缓冲区中复制数据
  void MemcpyToBuffer(char *pVoid, unsigned int size);
};
