#pragma once
#include <typeinfo>  // 提供 typeid 操作符，用于获取类型信息
#include "component_factory.h"  // 组件工厂类，用于注册和创建组件
#include "object_pool_collector.h"
#include "object_pool.h"        // 对象池类，用于内存管理

// 模板类：用于将组件注册到工厂并创建组件实例
template<typename T, typename... Targs>
class RegistToFactory
{
public:
    // 构造函数：在组件工厂中注册组件类型 T，并关联对应的创建函数
    RegistToFactory()
    {
        // 将组件类型 T 的名称和其创建函数关联，并注册到 ComponentFactory 中
        ComponentFactory<Targs...>::GetInstance()->Regist(typeid(T).name(), CreateComponent);
    }

    // 静态成员函数：用于创建组件实例，调用对象池进行内存分配
    static T* CreateComponent(SystemManager* pSysMgr, Targs... args)
    {
        // 从系统管理器获取对象池收集器
        auto pCollector = pSysMgr->GetPoolCollector();

        // 从收集器中获取对应组件 T 的对象池
        auto pPool = (DynamicObjectPool<T>*)pCollector->GetPool<T>();

        // 从对象池中分配一个 T 类型的对象，并传递系统管理器和参数
        return pPool->MallocObject(pSysMgr, std::forward<Targs>(args)...);
    }
};
