#pragma once
#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <sstream>

#include "cache_refresh.h"
#include "log4_help.h"
#include "object_pool_interface.h"
#include "sn_object.h"
#include "system_manager.h"

template <typename T> class DynamicObjectPool : public IDynamicObjectPool {
public:
  ~DynamicObjectPool();

  template <typename... Targs>
  T *MallocObject(SystemManager *pSysMgr, Targs... args);

  virtual void Update() override;
  void FreeObject(IComponent *pObj) override;

  void Show() override;

private:
  std::queue<T *> _free;
  CacheRefresh<T> _objInUse;
};

template <typename T> DynamicObjectPool<T>::~DynamicObjectPool() {
  std::cout << "dispose object pool. type:" << typeid(T).name() << std::endl;

  while (_free.size() > 0) {
    auto iter = _free.front();
    delete iter;
    _free.pop();
  }
}

template <typename T>
template <typename... Targs>
T *DynamicObjectPool<T>::MallocObject(SystemManager *pSys, Targs... args) {
  if (_free.size() == 0) {
    if (T::IsSingle()) {
      T *pObj = new T();
      pObj->SetPool(this);
      _free.push(pObj);
    } else {
      for (int index = 0; index < 50; index++) {
        T *pObj = new T();
        pObj->SetPool(this);
        _free.push(pObj);
      }
    }
  }

  auto pObj = _free.front();
  _free.pop();

  pObj->ResetSN();
  pObj->SetPool(this);
  pObj->SetSystemManager(pSys);
  pObj->Awake(std::forward<Targs>(args)...);

  _objInUse.GetAddCache()->push_back(pObj);
  return pObj;
}

template <typename T> void DynamicObjectPool<T>::Update() {
  std::list<T *> freeObjs;
  if (_objInUse.CanSwap()) {
    freeObjs = _objInUse.Swap();
  }
  for (auto one : freeObjs) {
    _free.push(one);
  }
}

template <typename T>
inline void DynamicObjectPool<T>::FreeObject(IComponent *pObj) {
  _objInUse.GetReaderCache()->emplace_back(dynamic_cast<T *>(pObj));
}

template <typename T> void DynamicObjectPool<T>::Show() {
  auto count = _objInUse.GetReaderCache()->size() +
               _objInUse.GetAddCache()->size() +
               _objInUse.GetRemoveCache()->size();

  std::stringstream log;
  log << " total:" << std::setw(5) << std::setfill(' ') << _free.size() + count
      << "    free:" << std::setw(5) << std::setfill(' ') << _free.size()
      << "    use:" << std::setw(5) << std::setfill(' ') << count << "    "
      << typeid(T).name();

  LOG_DEBUG(log.str().c_str());
}
