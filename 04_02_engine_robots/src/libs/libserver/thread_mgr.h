#pragma once

#include "common.h"
#include "singleton.h"
#include "thread.h"
#include <map>
#include <mutex>

// 前置声明类
/**
前置声明类主要在减少编译依赖、
解决循环依赖和减少编译时间等情况下使用。
它有助于保持代码的模块化和高效编译，
但在需要完整类定义的情况下，
仍然需要包含相应的头文件。
 */
class Packet;
class ThreadObject;
class Network;

// ThreadMgr类继承了Singleton和IDisposable，
// Singleton是单例模式，IDisposable是可释放资源的接口
class ThreadMgr : public Singleton<ThreadMgr>, public ThreadObjectList {
public:
  // 构造函数，初始化ThreadMgr
  ThreadMgr();

  // 启动所有线程
  void StartAllThread();

  // 检查是否是游戏循环线程
  bool IsGameLoop();

  // 创建新线程
  void NewThread();

  // 将对象添加到线程中
  bool AddObjToThread(ThreadObject *obj);

  // 根据应用类型将网络对象添加到线程中
  void AddNetworkToThread(APP_TYPE appType, Network *pNetwork);

  // 分发数据包
  void DispatchPacket(Packet *pPacket);

  // 发送数据包
  void SendPacket(Packet *pPacket);

  // 释放资源
  void Dispose();

private:
  // 根据应用类型获取网络对象
  Network *GetNetwork(APP_TYPE appType);

private:
  // 最后一个线程的序列号，初始为0
  uint64 _lastThreadSn{0};

  // 用于保护线程映射的互斥锁
  std::mutex _thread_lock;

  // 线程映射，使用线程序列号作为键
  std::map<uint64, Thread *> _threads;

  // 用于保护网络定位器映射的互斥锁
  std::mutex _locator_lock;

  // 网络定位器映射，使用应用类型作为键
  std::map<APP_TYPE, Network *> _networkLocator;
};
