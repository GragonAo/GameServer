#include "component.h"
#include "entity.h"
#include "system_manager.h"

void IComponent::SetParent(IEntity *pObj) { _parent = pObj; }

void IComponent::SetPool(IDynamicObjectPool *pPool) { _pPool = pPool; }

void IComponent::SetSystemManager(SystemManager *pSys) {
  _pSystemManager = pSys;
}

IEntity *IComponent::GetParent() const { return _parent; }

SystemManager *IComponent::GetSystemManager() const { return _pSystemManager; }

void IComponent::ComponentBackToPool() {

  BackToPool();

  if (_pPool != nullptr) {
    _pPool->FreeObject(this);
    _pPool = nullptr;
  }

  _sn = 0;
  _parent = nullptr;
  _pSystemManager = nullptr;
}