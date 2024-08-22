#pragma once
#include "disposable.h"
#include "object_pool.h"
#include "system.h"
#include "component.h"
#include <utility>

class SystemManager;

class DynamicObjectPoolCollector : public IDisposable {
public:
  DynamicObjectPoolCollector(SystemManager *pSys);
  void Dispose() override;
  template <class T> IDynamicObjectPool *GetPool();

  void Update();
  void Show();

private:
  std::map<uint64, IDynamicObjectPool *> _pools;
  SystemManager *_pSystemManager{nullptr};
};

template <class T> IDynamicObjectPool *DynamicObjectPoolCollector::GetPool() {
  const auto typeHasCode = typeid(T).hash_code();
  auto iter = _pools.find(typeHasCode);
  if (iter != _pools.end()) {
    return iter->second;
  }
  auto pPool = new DynamicObjectPool<T>();
  _pools.insert(std::make_pair(typeHasCode, pPool));
  return pPool;
}