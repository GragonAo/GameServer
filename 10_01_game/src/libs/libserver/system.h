#pragma once
#include "disposable.h" // 包含 IDisposable 接口的定义

class EntitySystem; // 前置声明，用于代表实体系统

// ISystem 类：基础系统接口，继承自 IDisposable
class ISystem : public IDisposable
{
protected:
    // 构造函数设为 protected，防止直接实例化
    ISystem() = default;

public:
    // 虚析构函数，确保派生类可以正确析构
    virtual ~ISystem() = default;

    // 实现自 IDisposable 的 Dispose 方法，默认空实现
    void Dispose() override {}

    // 虚函数 Update，可以由派生类实现，默认空实现
    virtual void Update(EntitySystem* pEntities) {}
};

// IAwakeSystem 模板类：带有唤醒功能的系统接口，继承自 ISystem
// TArgs 表示唤醒时所需的参数类型，可以支持多个参数
template <typename... TArgs>
class IAwakeSystem : virtual public ISystem
{
public:
    // 默认构造函数
    IAwakeSystem() = default;

    // 虚析构函数，确保派生类可以正确析构
    virtual ~IAwakeSystem() = default;

    // 纯虚函数 Awake，派生类必须实现，用于系统的初始化或唤醒操作
    virtual void Awake(TArgs... args) = 0;

    // 静态函数 IsSingle，返回 true 表示该系统是单例
    static bool IsSingle() { return true; }
};

// IAwakeFromPoolSystem 模板类：从对象池中唤醒的系统接口，继承自 ISystem
// TArgs 表示唤醒时所需的参数类型，可以支持多个参数
template <typename... TArgs>
class IAwakeFromPoolSystem : virtual public ISystem
{
public:
    // 默认构造函数
    IAwakeFromPoolSystem() = default;

    // 虚析构函数，确保派生类可以正确析构
    virtual ~IAwakeFromPoolSystem() = default;

    // 纯虚函数 Awake，派生类必须实现，用于系统的初始化或唤醒操作
    virtual void Awake(TArgs... args) = 0;

    // 静态函数 IsSingle，返回 false 表示该系统不是单例，可能存在多个实例
    static bool IsSingle() { return false; }
};
