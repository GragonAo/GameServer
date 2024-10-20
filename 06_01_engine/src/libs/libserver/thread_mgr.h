#pragma once

#include "common.h"
#include "entity_system.h"
#include "singleton.h"
#include "thread.h"
#include <map>
#include <mutex>
#include "component_factory.h"
#include "regist_to_factory.h"
// 前置声明类
/**
前置声明类主要在减少编译依赖、
解决循环依赖和减少编译时间等情况下使用。
它有助于保持代码的模块化和高效编译，
但在需要完整类定义的情况下，
仍然需要包含相应的头文件。
 */
class Packet;

// ThreadMgr类继承了Singleton和IDisposable，
// Singleton是单例模式，IDisposable是可释放资源的接口
class ThreadMgr : public Singleton<ThreadMgr>, public EntitySystem {
public:
  // 构造函数，初始化ThreadMgr
  ThreadMgr();

  // 启动所有线程
  void StartAllThread();
  void Update() override;

  bool IsStopAll();
  bool IsDisposeAll();

  // 创建新线程
  void CreateThread();

  template <class T, typename... TArgs> void CreateComponent(TArgs... args);

  // 分发数据包
  void DispatchPacket(Packet *pPacket);

  // 释放资源
  void Dispose() override;

private:
  template <typename... Args>
  void AnalyseParam(Proto::CreateComponent &proto, int value, Args... args);
  template <typename... Args>
  void AnalyseParam(Proto::CreateComponent &proto, std::string value,
                    Args... args);

  void AnalyseParam(Proto::CreateComponent &proto) {}

private:
  std::vector<Thread *> _threads;
  size_t _threadIndex{0};

  std::mutex _create_lock;
  CacheSwap<Packet> _createPackets;
};

template <class T, typename... TArgs>
inline void ThreadMgr::CreateComponent(TArgs... args) {
  std::lock_guard<std::mutex> guard(_create_lock);

  const std::string className = typeid(T).name();
  if (!ComponentFactory<TArgs...>::GetInstance()->IsRegisted(className)) {
    RegistToFactory<T, TArgs...>();
  }

  Proto::CreateComponent proto;
  proto.set_class_name(className.c_str());
  AnalyseParam(proto, std::forward<TArgs>(args)...);

  auto pCreatePacket = new Packet(Proto::MsgId::MI_CreateComponent, 0);
  pCreatePacket->SerializeToBuffer(proto);
  _createPackets.GetWriterCache()->emplace_back(pCreatePacket);
}

template <typename... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent &proto, int value,
                             Args... args) {
  auto pProtoParam = proto.mutable_params()->Add();
  pProtoParam->set_type(Proto::CreateComponentParam::Int);
  pProtoParam->set_int_param(value);
  AnalyseParam(proto, std::forward<Args>(args)...);
}

template <typename... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent &proto, std::string value,
                             Args... args) {
  auto pProtoParam = proto.mutable_params()->Add();
  pProtoParam->set_type(Proto::CreateComponentParam::String);
  pProtoParam->set_string_param(value.c_str());
  AnalyseParam(proto, std::forward<Args>(args)...);
}