#pragma once
#include "component_factory.h"
#include "object_pool.h"
#include <typeinfo>
#include "object_pool_collector.h"
template <typename T, typename... Targs> class RegistToFactory {
public:
  RegistToFactory() {
    ComponentFactory<Targs...>::GetInstance()->Regist(typeid(T).name(),
                                                      CreateComponent);
  }
  static T *CreateComponent(SystemManager* pSysMgr,Targs... args) {
    auto pCollector = pSysMgr->GetPoolCollector();
    auto pPool = (DynamicObjectPool<T>*) pCollector->GetPool<T>();
    return pPool->MallocObject(pSysMgr,std::forward<Targs>(args)...);
  }
};