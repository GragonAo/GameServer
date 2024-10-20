#pragma once

#include <map>
#include <list>
#include <memory>
#include <algorithm>
#include <queue>

#include "component.h"
#include "entity_system.h"

// IEntity 类继承自 IComponent，表示游戏中的实体
class IEntity : public IComponent
{
public:
    virtual ~IEntity() = default; // 虚析构函数，以确保派生类的析构函数被调用

    // 将组件返回到对象池
    void ComponentBackToPool() override;

    // 添加组件到实体中，支持可变参数
    template <class T, typename... TArgs>
    T* AddComponent(TArgs... args);

    // 获取实体中的组件
    template<class T>
    T* GetComponent();

    // 移除指定类型的组件
    template<class T>
    void RemoveComponent();

    // 移除指定组件
    void RemoveComponent(IComponent* pObj);

protected:
    // 存储组件，键为类型的哈希值，值为组件指针
    std::map<uint64, IComponent*> _components;
};

// 添加组件的实现
template <class T, typename... TArgs>
inline T* IEntity::AddComponent(TArgs... args)
{
    const auto typeHashCode = typeid(T).hash_code(); // 获取组件类型的哈希值

    // 检查是否已经添加过该类型的组件
    // if (_components.find(typeHashCode) != _components.end())
    // {
    //     LOG_ERROR("Add same component. type:" << typeid(T).name()); // 记录错误日志
    //     return nullptr; // 返回 nullptr 表示添加失败
    // }

    // 从系统管理器中添加组件
    T* pComponent = _pSystemManager->GetEntitySystem()->AddComponent<T>(std::forward<TArgs>(args)...);
    pComponent->SetParent(this); // 设置组件的父实体为当前实体
    _components.insert(std::make_pair(typeHashCode, pComponent)); // 将组件存储在 _components 中
    return pComponent; // 返回添加的组件指针
}

// 获取组件的实现
template<class T>
T* IEntity::GetComponent()
{
    const auto typeHashCode = typeid(T).hash_code(); // 获取组件类型的哈希值
    const auto iter = _components.find(typeHashCode); // 在 _components 中查找

    if (iter == _components.end())
        return nullptr; // 如果没有找到，返回 nullptr

    return dynamic_cast<T*>(iter->second); // 使用 dynamic_cast 转换类型并返回组件指针
}

// 移除组件的实现
template<class T>
void IEntity::RemoveComponent()
{
    const auto typeHashCode = typeid(T).hash_code(); // 获取组件类型的哈希值
    const auto iter = _components.find(typeHashCode); // 在 _components 中查找

    if (iter == _components.end())
    {
        LOG_ERROR("Entity RemoveComponent error. not find. className:" << typeid(T).name()); // 记录错误日志
        return; // 如果没有找到，返回
    }

    auto pComponent = iter->second; // 获取要移除的组件
    RemoveComponent(pComponent); // 调用另一个重载的 RemoveComponent 方法
}

// Entity 类继承自 IEntity，表示具体的实体类型
template<class T>
class Entity : public IEntity
{
public:
    // 获取实体类型的名称
    const char* GetTypeName() override;
    // 获取实体类型的哈希值
    uint64 GetTypeHashCode() override;
};

// 获取实体类型名称的实现
template <class T>
const char* Entity<T>::GetTypeName()
{
    return typeid(T).name(); // 返回类型的名称
}

// 获取实体类型哈希值的实现
template <class T>
uint64 Entity<T>::GetTypeHashCode()
{
    return typeid(T).hash_code(); // 返回类型的哈希值
}
