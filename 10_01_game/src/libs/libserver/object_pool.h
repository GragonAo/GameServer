#pragma once
#include <queue>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "sn_object.h"              // 包含序列号对象的声明
#include "object_pool_interface.h"  // 包含对象池接口的声明
#include "cache_refresh.h"          // 包含缓存刷新机制的声明
#include "log4_help.h"              // 包含日志帮助工具
#include "system_manager.h"         // 包含系统管理器的声明

// 动态对象池类模板，负责对象的分配与回收
template <typename T>
class DynamicObjectPool : public IDynamicObjectPool
{
public:
    // 析构时调用，释放池中对象
    void Dispose() override;

    // 分配对象，传入参数并调用对象的 Awake 方法
    template<typename ...Targs>
    T* MallocObject(SystemManager* pSys, Targs... args);

    // 更新对象池，处理过期对象
    virtual void Update() override;

    // 释放对象，标记对象已被释放
    virtual void FreeObject(IComponent* pObj) override;

    // 显示当前对象池状态
    virtual void Show() override;

protected:
    std::queue<T*> _free;               // 空闲对象队列
    CacheRefresh<T> _objInUse;          // 正在使用的对象缓存

#if _DEBUG
    int _totalCall{ 0 };                // 调试模式下，记录总调用次数
#endif
};

// Dispose 方法实现，释放对象池中的所有对象
template <typename T>
void DynamicObjectPool<T>::Dispose()
{
    // 检查当前是否有对象仍在使用中
    if (_objInUse.Count() > 0)
    {
        std::cout << "delete pool. " << typeid(T).name() << " count:" << _objInUse.Count() << std::endl;
    }

    // 释放空闲队列中的所有对象
    while (!_free.empty())
    {
        auto obj = _free.front();
        delete obj;
        _free.pop();
    }
}

// MallocObject 方法实现，分配新对象或从空闲队列中取出对象
template <typename T>
template <typename ... Targs>
T* DynamicObjectPool<T>::MallocObject(SystemManager* pSys, Targs... args)
{
    // 如果空闲队列为空，根据对象是否为单例进行分配
    if (_free.empty())
    {
        if (T::IsSingle())
        {
            // 如果对象为单例，则创建一个对象
            T* pObj = new T();
            pObj->ResetSN(true);
            pObj->SetPool(this);
            _free.push(pObj);
        }
        else
        {
            // 如果对象非单例，一次性创建50个对象
            for (int index = 0; index < 50; index++)
            {
                T* pObj = new T();
                pObj->ResetSN(true);
                pObj->SetPool(this);
                _free.push(pObj);
            }
        }
    }

#if _DEBUG
    _totalCall++;  // 调试模式下，记录调用次数
#endif

    // 取出空闲对象
    auto pObj = _free.front();
    _free.pop();

    // 检查对象的序列号是否为0，如果不是，记录错误日志
    if (pObj->GetSN() != 0)
    {
        LOG_ERROR("failed to create type:" << typeid(T).name() << " sn != 0. sn:" << pObj->GetSN());
    }

    // 初始化对象的属性，并调用 Awake 方法
    pObj->ResetSN();
    pObj->SetPool(this);
    pObj->SetSystemManager(pSys);
    pObj->Awake(std::forward<Targs>(args)...);

#if LOG_SYSOBJ_OPEN
    LOG_SYSOBJ("*[pool] awake obj. obj sn:" << pObj->GetSN() << " type:" << pObj->GetTypeName() << " thead id:" << std::this_thread::get_id());
#endif

    // 将对象标记为正在使用
    _objInUse.AddObj(pObj);
    return pObj;
}

// Update 方法实现，处理使用中的对象是否可交换回空闲队列
template <typename T>
void DynamicObjectPool<T>::Update()
{
    // 如果缓存中的对象可交换，则将其加入空闲队列
    if (_objInUse.CanSwap())
    {
        _objInUse.Swap(&_free);
    }
}

// FreeObject 方法实现，将对象标记为已释放
template<typename T>
void DynamicObjectPool<T>::FreeObject(IComponent* pObj)
{
    // 检查对象的序列号是否为0，如果是则记录错误日志
    if (pObj->GetSN() == 0)
    {
        LOG_ERROR("free obj sn == 0. type:" << typeid(T).name());
        return;
    }

#if LOG_SYSOBJ_OPEN
    LOG_SYSOBJ("*[pool] free obj. obj sn:" << pObj->GetSN() << " type:" << pObj->GetTypeName() << " thead id:" << std::this_thread::get_id());
#endif

    // 将对象从正在使用的缓存中移除
    _objInUse.RemoveObj(pObj->GetSN());
}

// Show 方法实现，显示对象池的当前状态，包括空闲对象和使用中对象的数量
template <typename T>
void DynamicObjectPool<T>::Show()
{
    std::stringstream log;
    log << " total:" << std::setw(5) << std::setfill(' ') << _free.size() + _objInUse.Count()

#if _DEBUG
        << "    call:" << std::setw(5) << std::setfill(' ') << _totalCall  // 调试模式下显示调用次数
#endif

        << "    free:" << std::setw(5) << std::setfill(' ') << _free.size()  // 显示空闲对象数量
        << "    use:" << std::setw(5) << std::setfill(' ') << _objInUse.Count()  // 显示使用中对象数量
        << "    " << typeid(T).name();  // 显示对象类型名

    LOG_DEBUG(log.str().c_str());  // 记录日志
}
