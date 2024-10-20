#pragma once

#include <mutex>              // 提供线程安全锁
#include <vector>             // 存储线程相关数据
#include "common.h"           // 常见的全局定义
#include "app_type.h"
#include "thread.h"           // 线程类
#include "cache_swap.h"       // 用于缓存交换的工具类
#include "singleton.h"        // 单例模式模板类
#include "entity_system.h"    // 实体系统类
#include "component_factory.h"// 组件工厂类，用于组件的创建
#include "regist_to_factory.h"// 组件注册工具
#include "message_system_help.h" // 消息系统帮助类
#include "thread_collector.h" // 线程收集器
#include "thread_type.h"      // 线程类型定义

// ThreadMgr类：继承自单例模板和SystemManager，管理线程的创建和销毁
class ThreadMgr :public Singleton<ThreadMgr>, public SystemManager
{
public:
    ThreadMgr();                                // 构造函数
    void InitializeThread();                    // 初始化所有线程
    void CreateThread(ThreadType iType, int num);// 根据类型和数量创建线程
    
    void InitializeGlobalComponent(APP_TYPE ppType, int appId); // 初始化全局组件
    void Update() override;                     // 更新系统管理器中的组件
    void UpdateCreatePacket();                  // 更新组件创建的消息包
    void UpdateDispatchPacket();                // 更新消息包的分发

    bool IsStopAll();                           // 检查所有线程是否停止
    void DestroyThread();                       // 销毁所有线程
    bool IsDestroyAll();                        // 检查所有线程是否已经销毁

    void Dispose() override;                    // 重写的清理方法

    // 创建组件的方法重载
    template<class T, typename ...TArgs>
    void CreateComponent(TArgs... args);

    template<class T, typename ...TArgs>
    void CreateComponent(bool isToAllThead, TArgs... args);

    template<class T, typename ...TArgs>
    void CreateComponent(ThreadType iType, bool isToAllThead, TArgs... args);

    // 消息分发方法
    void DispatchPacket(Packet* pPacket);

private:
    // 递归分析创建组件参数
    template <typename...Args>
    void AnalyseParam(Proto::CreateComponent& proto, int value, Args...args);
    
    template <typename...Args>
    void AnalyseParam(Proto::CreateComponent& proto, std::string value, Args...args);

    // 分析参数的递归终止条件
    void AnalyseParam(Proto::CreateComponent& proto)
    {
    }

private:
    std::map<ThreadType, ThreadCollector*> _threads; // 存储线程收集器

    // 用于创建组件的消息锁和缓存
    std::mutex _create_lock;
    CacheSwap<Packet> _createPackets;

    // 消息包分发的锁和缓存
    std::mutex _packet_lock;
    CacheSwap<Packet> _packets;
};

// 创建组件的模板函数，针对不同参数进行重载
template<class T, typename ...TArgs>
void ThreadMgr::CreateComponent(TArgs ...args)
{
    CreateComponent<T>(LogicThread, false, std::forward<TArgs>(args)...);
}

template<class T, typename ...TArgs>
void ThreadMgr::CreateComponent(bool isToAllThead, TArgs ...args)
{
    CreateComponent<T>(LogicThread, isToAllThead, std::forward<TArgs>(args)...);
}

template<class T, typename ...TArgs>
void ThreadMgr::CreateComponent(ThreadType iType, bool isToAllThead, TArgs ...args)
{
    // 线程安全的锁定创建过程
    std::lock_guard<std::mutex> guard(_create_lock);

    const std::string className = typeid(T).name(); // 获取组件的类名
    // 如果组件尚未注册到工厂中，则进行注册
    if (!ComponentFactory<TArgs...>::GetInstance()->IsRegisted(className))
    {
        RegistToFactory<T, TArgs...>();
    }

    // 创建组件的消息包，并填充其参数
    Proto::CreateComponent proto;
    proto.set_thread_type((int)iType);
    proto.set_class_name(className.c_str());
    proto.set_is_to_all_thread(isToAllThead);
    AnalyseParam(proto, std::forward<TArgs>(args)...);

    // 将消息包序列化并加入到创建消息队列
    auto pCreatePacket = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_CreateComponent, nullptr);
    pCreatePacket->SerializeToBuffer(proto);
    _createPackets.GetWriterCache()->emplace_back(pCreatePacket);
}

// 分析参数模板：处理整型参数
template<typename ... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent& proto, int value, Args... args)
{
    // 将参数添加到创建组件的参数列表中
    auto pProtoParam = proto.mutable_params()->Add();
    pProtoParam->set_type(Proto::CreateComponentParam::Int);  // 参数类型设置为整型
    pProtoParam->set_int_param(value);                        // 设置整型值
    AnalyseParam(proto, std::forward<Args>(args)...);         // 递归处理剩余参数
}

// 分析参数模板：处理字符串参数
template<typename ... Args>
void ThreadMgr::AnalyseParam(Proto::CreateComponent& proto, std::string value, Args... args)
{
    auto pProtoParam = proto.mutable_params()->Add();
    pProtoParam->set_type(Proto::CreateComponentParam::String); // 参数类型设置为字符串
    pProtoParam->set_string_param(value.c_str());               // 设置字符串值
    AnalyseParam(proto, std::forward<Args>(args)...);           // 递归处理剩余参数
}
