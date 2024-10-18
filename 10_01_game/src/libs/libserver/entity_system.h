#pragma once
#include "component.h"

#include "disposable.h"
#include "component_factory.h"
#include "object_pool.h"
#include "component_collections.h"
#include "system_manager.h"
#include "log4_help.h"
#include "object_pool_collector.h"

// Packet 类的前置声明
class Packet;

// EntitySystem 类负责管理游戏中的实体组件
class EntitySystem : public IDisposable
{
public:
    friend class ConsoleThreadComponent; // 允许 ConsoleThreadComponent 访问私有成员

    // 构造函数，接收系统管理器指针
    EntitySystem(SystemManager* pMgr);
    
    // 虚析构函数
    virtual ~EntitySystem();

    // 添加组件，支持可变参数
    template <class T, typename... TArgs>
    T* AddComponent(TArgs... args);

    // 通过组件名称添加组件
    template <typename... TArgs>
    IComponent* AddComponentByName(std::string className, TArgs... args);

    // 获取指定类型的组件
    template <class T>
    T* GetComponent();

    // 移除指定的组件
    void RemoveComponent(IComponent* pObj);

    // 获取指定类型的组件集合
    template<class T>
    ComponentCollections* GetComponentCollections();

    // 更新系统
    void Update();

    // 释放资源
    void Dispose() override;

private:
    // 添加组件的实现
    template <class T>
    void AddComponent(T* pComponent);

    // 存储组件集合，键为组件类型的哈希值
    std::map<uint64, ComponentCollections*> _objSystems;

private:
    SystemManager* _systemManager; // 系统管理器指针
};

// 添加组件的实现
template<class T>
inline void EntitySystem::AddComponent(T* pComponent)
{
    const auto typeHashCode = pComponent->GetTypeHashCode(); // 获取组件类型的哈希值

#if LOG_SYSOBJ_OPEN
    // 记录添加组件的日志
    LOG_SYSOBJ("*[sys] add obj. obj sn:" << pComponent->GetSN() << " type:" << pComponent->GetTypeName() << " thread id:" << std::this_thread::get_id());
#endif

    // 查找组件集合
    auto iter = _objSystems.find(typeHashCode);
    if (iter == _objSystems.end())
    {
        // 如果组件类型不存在，则创建新的组件集合
        _objSystems[typeHashCode] = new ComponentCollections(pComponent->GetTypeName());
    }

    // 获取对应类型的组件集合并添加组件
    auto pEntities = _objSystems[typeHashCode];
    pEntities->Add(dynamic_cast<IComponent*>(pComponent)); // 动态转换并添加
    pComponent->SetSystemManager(_systemManager); // 设置系统管理器
}

// 添加组件的实现，支持可变参数
template <class T, typename ... TArgs>
T* EntitySystem::AddComponent(TArgs... args)
{
    // 获取对象池收集器
    auto pCollector = _systemManager->GetPoolCollector();
    auto pPool = (DynamicObjectPool<T>*)pCollector->GetPool<T>(); // 获取特定类型的对象池

    // 从对象池分配新组件
    T* pComponent = pPool->MallocObject(_systemManager, std::forward<TArgs>(args)...);
    if (pComponent == nullptr)
        return nullptr; // 如果分配失败，返回 nullptr

    // 添加组件
    AddComponent(pComponent);
    return pComponent; // 返回分配的组件
}

// 通过名称添加组件的实现
template<typename ...TArgs>
inline IComponent* EntitySystem::AddComponentByName(std::string className, TArgs ...args)
{
    // 使用组件工厂创建组件
    auto pComponent = ComponentFactory<TArgs...>::GetInstance()->Create(_systemManager, className, std::forward<TArgs>(args)...);
    if (pComponent == nullptr)
        return nullptr; // 如果创建失败，返回 nullptr

    // 添加组件
    AddComponent(pComponent);
    return pComponent; // 返回创建的组件
}

// 获取组件的实现
template <class T>
T* EntitySystem::GetComponent()
{
    const auto typeHashCode = typeid(T).hash_code(); // 获取组件类型的哈希值
    auto iter = _objSystems.find(typeHashCode); // 查找组件集合

    if (iter == _objSystems.end())
        return nullptr; // 如果没有找到，返回 nullptr

    // 返回指定类型的组件
    return dynamic_cast<T*>(iter->second->Get());
}

// 获取组件集合的实现
template<class T>
inline ComponentCollections* EntitySystem::GetComponentCollections()
{
    const auto typeHashCode = typeid(T).hash_code(); // 获取组件类型的哈希值
    auto iter = _objSystems.find(typeHashCode); // 查找组件集合

    if (iter == _objSystems.end())
    {
        // 如果没有找到，返回 nullptr
        return nullptr;
    }

    return iter->second; // 返回找到的组件集合
}
