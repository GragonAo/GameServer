#pragma once

#include "component.h"
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

//组件工厂 
template <typename... Targs> class ComponentFactory {
public:

  //生产组件时需要调用的创建回调
  typedef std::function<IComponent *(SystemManager*, Targs...)>
      FactoryFunction;

  //单例，意味着负责整个进程中的组件生产
  static ComponentFactory<Targs...>* GetInstance() {
    if (_pInstance == nullptr) {
      _pInstance = new ComponentFactory<Targs...>();
    }
    return _pInstance;
  }

  //注册组件的生产方法，每个组件只有一个生产的方法
  bool Regist(const std::string &className, FactoryFunction pFunc) {
    std::lock_guard<std::mutex> guard(_lock);
    if (_map.find(className) != _map.end())
      return false;
    _map.insert(std::make_pair(className, pFunc));
    return true;
  }

  //查看组件是否有注册到组件工厂中
  bool IsRegisted(const std::string &className) {
    std::lock_guard<std::mutex> guard(_lock);
    return _map.find(className) != _map.end();
  }

  //当组件存在工厂中时负责生产对应的组件
  IComponent *Create(SystemManager *pSysMgr, const std::string className,
                     Targs... args) {
    _lock.lock();
    auto iter = _map.find(className);
    if (iter == _map.end()) {
      std::cout
          << "CompontFactory Create failed. can't find component. className:"
          << className.c_str() << std::endl;
      return nullptr;
    }
    auto fun = iter->second;
    _lock.unlock();

    return fun(pSysMgr,std::forward<Targs>(args)...);
  }

private:
  static ComponentFactory<Targs...> *_pInstance;
  std::map<std::string, FactoryFunction> _map;
  std::mutex _lock;
};

template <typename... Targs>
ComponentFactory<Targs...> *ComponentFactory<Targs...>::_pInstance = nullptr;