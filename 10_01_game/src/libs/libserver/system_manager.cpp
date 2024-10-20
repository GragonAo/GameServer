#include "system_manager.h"

#include "create_component.h"       // 包含 CreateComponentC 组件的声明
#include "message_system.h"         // 包含 MessageSystem 系统的声明
#include "entity_system.h"          // 包含 EntitySystem 系统的声明
#include "update_system.h"          // 包含 UpdateSystem 系统的声明

#include "console_thread_component.h"  // 包含 ConsoleThreadComponent 组件的声明
#include "object_pool_collector.h"     // 包含 DynamicObjectPoolCollector 对象池管理的声明
#include "timer_component.h"           // 包含 TimerComponent 计时组件的声明

#include <thread>                      // 用于获取当前线程 ID

// 构造函数，初始化关键系统及随机数生成器
SystemManager::SystemManager()
{
    // 实体系统和消息系统的初始化
    _pEntitySystem = new EntitySystem(this);
    _pMessageSystem = new MessageSystem(this);

    // 向系统列表添加 UpdateSystem，负责更新操作
    _systems.emplace_back(new UpdateSystem());

    // 生成随机数种子，使用当前线程 ID
    std::stringstream strStream;
    strStream << std::this_thread::get_id();  // 将当前线程 ID 转换为字符串
    std::string idstr = strStream.str();      // 获取线程 ID 字符串
    std::seed_seq seed1(idstr.begin(), idstr.end());  // 根据线程 ID 生成种子序列
    std::minstd_rand0 generator(seed1);       // 创建最小标准随机数生成器
    _pRandomEngine = new std::default_random_engine(generator());  // 初始化默认随机数引擎

    // 初始化对象池收集器
    _pPoolCollector = new DynamicObjectPoolCollector(this);
}

// 初始化实体系统组件，传入线程类型
void SystemManager::InitComponent(ThreadType threadType)
{
    // 向实体系统中添加 TimerComponent、CreateComponentC 和 ConsoleThreadComponent
    _pEntitySystem->AddComponent<TimerComponent>();
    _pEntitySystem->AddComponent<CreateComponentC>();
    _pEntitySystem->AddComponent<ConsoleThreadComponent>(threadType);
}

// 更新函数，负责调用所有系统的更新操作
void SystemManager::Update()
{
    // 更新对象池收集器
    _pPoolCollector->Update();

    // 更新实体系统和消息系统
    _pEntitySystem->Update();
    _pMessageSystem->Update(_pEntitySystem);

    // 迭代系统列表，逐个更新每个系统
    for (auto iter = _systems.begin(); iter != _systems.end(); ++iter)
    {
        (*iter)->Update(_pEntitySystem);
    }
}

// 销毁函数，释放所有资源并销毁系统
void SystemManager::Dispose()
{
    // 销毁并清空系统列表
    for (auto one : _systems)
    {
        one->Dispose();
        delete one;
    }
    _systems.clear();

    // 释放随机数引擎
    delete _pRandomEngine;
    _pRandomEngine = nullptr;

    // 释放消息系统
    _pMessageSystem->Dispose();
    delete _pMessageSystem;
    _pMessageSystem = nullptr;

    // 释放实体系统
    _pEntitySystem->Dispose();
    delete _pEntitySystem;
    _pEntitySystem = nullptr;

    // 在对象池管理器销毁前进行更新，确保所有待释放的对象已被处理
    _pPoolCollector->Update();
    _pPoolCollector->Dispose();
    delete _pPoolCollector;
    _pPoolCollector = nullptr;
}

// 获取随机数生成引擎
std::default_random_engine* SystemManager::GetRandomEngine() const
{
    return _pRandomEngine;
}

// 获取消息系统
MessageSystem* SystemManager::GetMessageSystem() const
{
    return _pMessageSystem;
}

// 获取实体系统
EntitySystem* SystemManager::GetEntitySystem() const
{
    return _pEntitySystem;
}

// 获取对象池收集器
DynamicObjectPoolCollector* SystemManager::GetPoolCollector() const
{
    return _pPoolCollector;
}
