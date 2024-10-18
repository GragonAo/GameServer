#pragma once

#include <mutex>
#include <vector>

#include "common.h"
#include "thread.h"
#include "cache_swap.h"
#include "singleton.h"
#include "entity_system.h"
#include "component_factory.h"
#include "regist_to_factory.h"
#include "message_system_help.h"
#include "thread_collector.h"
#include "thread_type.h"
#include "packet.h"


class ThreadMgr : public Singleton<ThreadMgr>, public SystemManager {
public:
  // 构造函数，初始化ThreadMgr
  ThreadMgr();
  void InitializeThread();
  void CreateThread(ThreadType iType, int num);

  void InitializeGloablComponent(APP_TYPE ppType, int appId);
  void Update() override;
  void UpdateCreatePacket();
  void UpdateDispatchPacket();

  bool IsStopAll();
  void DestroyThread();
  bool IsDisposeAll();
  // 释放资源
  void Dispose() override;

  template <class T, typename... TArgs> void CreateComponent(TArgs... args);


  template <class T, typename... Targs>
  void CreateComponent(bool isToAllThead, Targs... args);

  template <class T, typename... Targs>
  void CreateComponent(ThreadType iType,bool isToAllThead, Targs... args);

  // 分发数据包
  void DispatchPacket(Packet *pPacket);

private:
  template <typename... Args>
  void AnalyseParam(Proto::CreateComponent &proto, int value, Args... args);
  template <typename... Args>
  void AnalyseParam(Proto::CreateComponent &proto, std::string value,
                    Args... args);

  void AnalyseParam(Proto::CreateComponent &proto) {}

private:
  std::map<ThreadType, ThreadCollector *> _threads;

  std::mutex _create_lock;
  CacheSwap<Packet> _createPackets;

  std::mutex _packet_lock;
  CacheSwap<Packet> _packets;
};

template <class T, typename... TArgs>
void ThreadMgr::CreateComponent(TArgs... args) {
  CreateComponent<T>(LogicThread,false, std::forward<TArgs>(args)...);
}

template<class T, typename ...TArgs>
void ThreadMgr::CreateComponent(bool isToAllThead, TArgs ...args)
{
    CreateComponent<T>(LogicThread, isToAllThead, std::forward<TArgs>(args)...);
}

template <class T, typename... TArgs>
inline void ThreadMgr::CreateComponent(ThreadType iType,bool isToAllThead, TArgs... args) {
  std::lock_guard<std::mutex> guard(_create_lock);

  const std::string className = typeid(T).name();
  if (!ComponentFactory<TArgs...>::GetInstance()->IsRegisted(className)) {
    RegistToFactory<T, TArgs...>();
  }

  Proto::CreateComponent proto;
  proto.set_thread_type((int)iType);
  proto.set_class_name(className.c_str());
  proto.set_is_to_all_thread(isToAllThead);
  AnalyseParam(proto, std::forward<TArgs>(args)...);

  auto pCreatePacket =
      MessageSystemHelp::CreatePacket(Proto::MsgId::MI_CreateComponent, nullptr);
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