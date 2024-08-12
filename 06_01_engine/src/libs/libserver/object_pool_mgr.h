#pragma once

#include "disposable.h"
#include "object_pool_interface.h"
#include "singleton.h"
#include <list>
#include <mutex>

class DynamicObjectPoolMgr : public Singleton<DynamicObjectPoolMgr>,
                             public IDisposable {
public:
  void AddPool(IDynamicObjectPool *pPool);
  void Update();
  void Dispose() override;

private:
  std::mutex _lock;
  std::list<IDynamicObjectPool *> _pools;
};