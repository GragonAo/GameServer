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
  void Dispose() override;

  template <typename... Targs>
  T *MallocObject(SystemManager *pSysMgr, Targs... args);

  virtual void Update() override;
  void FreeObject(IComponent *pObj) override;

  void Show() override;

protected:
  std::queue<T *> _free;
  CacheRefresh<T> _objInUse;

  int _totalCall{0};
};

template <typename T> void DynamicObjectPool<T>::Dispose() {
  std::cout << "delete pool. type: " << typeid(T).name() << std::endl;

  if (_objInUse.Count() > 0) {
    std::cout << "type: " << typeid(T).name() << " count:" << _objInUse.Count()
              << std::endl;
  }

  while (!_free.empty()) {
    auto iter = _free.front();
    delete iter;
    _free.pop();
  }
}

template <typename T>
template <typename... Targs>
T *DynamicObjectPool<T>::MallocObject(SystemManager *pSys, Targs... args) {
  if (_free.empty()) {
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

  _totalCall++;

  auto pObj = _free.front();
  _free.pop();

  pObj->ResetSN();
  pObj->SetPool(this);
  pObj->SetSystemManager(pSys);
  pObj->Awake(std::forward<Targs>(args)...);

  _objInUse.AddObj(pObj);
  return pObj;
}

template <typename T> void DynamicObjectPool<T>::Update() {
  if (_objInUse.CanSwap()) {
    _objInUse.Swap(&_free);
  }
}

template <typename T>
inline void DynamicObjectPool<T>::FreeObject(IComponent *pObj) {
  if (pObj->GetSN() == 0) {
    LOG_ERROR("free obj sn == 0. type : " << typeid(T).name());
    return;
  }

  _objInUse.RemoveObj(pObj->GetSN());
}

template <typename T> void DynamicObjectPool<T>::Show() {
  std::stringstream log;
  log << " total:" << std::setw(5) << std::setfill(' ')
      << _free.size() + _objInUse.Count() << "    free:" << std::setw(5)
      << std::setfill(' ') << _free.size() << "    use:" << std::setw(5)
      << std::setfill(' ') << _objInUse.Count() << "    call:" << std::setw(5)
      << std::setfill(' ') << _totalCall << "    " << typeid(T).name();

  LOG_DEBUG(log.str().c_str());
}
