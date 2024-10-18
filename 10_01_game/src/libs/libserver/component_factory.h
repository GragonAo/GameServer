#pragma once
#include "component.h"
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include <mutex>  // 确保线程安全

template<typename ...Targs>
class ComponentFactory
{
public:
    // 定义一个工厂函数的类型，该函数接收 SystemManager 和可变参数，返回一个 IComponent 指针
    typedef std::function<IComponent*(SystemManager*, Targs...)> FactoryFunction;

    // 获取单例实例
    static ComponentFactory<Targs...>* GetInstance()
    {
        if (_pInstance == nullptr)
        {
            _pInstance = new ComponentFactory<Targs...>();
        }
        return _pInstance;
    }

    // 注册组件类的构造函数，className 为组件类名，pFunc 为组件的构造函数
    bool Regist(const std::string &className, FactoryFunction pFunc)
    {
        std::lock_guard<std::mutex> guard(_lock);  // 保证线程安全
        if (_map.find(className) != _map.end())
            return false;  // 如果组件已经注册，返回 false

        _map.insert(std::make_pair(className, pFunc));  // 注册组件
        return true;
    }

    // 判断类名为 className 的组件是否已经注册
    bool IsRegisted(const std::string &className)
    {
        std::lock_guard<std::mutex> guard(_lock);  // 保证线程安全
        return _map.find(className) != _map.end();
    }

    // 创建一个组件对象，根据类名 className 和传递的可变参数 args 调用对应的构造函数
    IComponent* Create(SystemManager* pSysMgr, const std::string className, Targs... args)
    {
        _lock.lock();  // 锁定，保证线程安全
        auto iter = _map.find(className);
        if (iter == _map.end())  // 如果类名不存在，输出错误信息
        {
            std::cout << "ComponentFactory Create failed. Can't find component. className:" << className.c_str() << std::endl;
            _lock.unlock();
            return nullptr;
        }
        auto fun = iter->second;  // 获取对应的工厂函数
        _lock.unlock();

        return fun(pSysMgr, std::forward<Targs>(args)...);  // 调用工厂函数创建对象
    }

private:
    // 单例模式的静态实例
    static ComponentFactory<Targs...>* _pInstance;

    // 保存所有注册的组件构造函数，键为类名
    std::map<std::string, FactoryFunction> _map;

    // 用于保证多线程安全的互斥锁
    std::mutex _lock;
};

// 静态成员变量初始化
template<typename ...Targs>
ComponentFactory<Targs...>* ComponentFactory<Targs...>::_pInstance = nullptr;
