#pragma once

#include "disposable.h" // 包含 IDisposable 接口定义
#include "system.h"     // 包含 ISystem 接口定义
#include "common.h"     // 包含常用定义
#include "thread_type.h"// 包含线程类型定义

#include <list>         // 用于管理系统的列表
#include <random>       // 随机数生成器使用的库

// 前置声明，避免循环依赖
class EntitySystem;
class MessageSystem;
class DynamicObjectPoolCollector;

// SystemManager 类：负责管理各类系统组件的生命周期和运行状态
class SystemManager : virtual public IDisposable
{
public:
    // 构造函数，初始化指针为 nullptr
    SystemManager();

    // 初始化组件，根据线程类型（ThreadType）设置
    void InitComponent(ThreadType threadType);

    // 更新函数，迭代所有管理的系统进行更新
    virtual void Update();

    // 销毁函数，重写自 IDisposable，用于释放资源
    void Dispose() override;

    // 获取消息系统指针
    MessageSystem* GetMessageSystem() const;

    // 获取实体系统指针
    EntitySystem* GetEntitySystem() const;

    // 获取对象池收集器指针
    DynamicObjectPoolCollector* GetPoolCollector() const;

    // 获取随机数引擎指针，用于随机数生成
    std::default_random_engine* GetRandomEngine() const;

protected:
    // 指向消息系统的指针
    MessageSystem* _pMessageSystem;

    // 指向实体系统的指针
    EntitySystem* _pEntitySystem;

    // 系统列表，保存管理的所有系统
    std::list<ISystem*> _systems;

    // 随机数生成引擎指针
    std::default_random_engine* _pRandomEngine;

    // 对象池收集器，用于管理对象池的分配和回收
    DynamicObjectPoolCollector* _pPoolCollector;
};
