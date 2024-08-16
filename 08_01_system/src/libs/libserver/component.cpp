#include "component.h"
#include "entity_system.h"
#include "entity.h"
void IComponent::SetParent(IEntity *pObj) {
    _parent = pObj;
}

void IComponent::SetPool(IDynamicObjectPool *pPool) {
    _pPool = pPool;
}

void IComponent::SetSystemManager(SystemManager *pSys) {
    _pSystemManager = pSys;
}

SystemManager *IComponent::GetSystemManager() const {
    return _pSystemManager;
}
void IComponent::ComponentBackToPool() {

    BackToPool();

    _sn = 0;
    _parent = nullptr;
    _pSystemManager = nullptr;
    _active = true;

    BackToPool();
    if(_pPool != nullptr)
        _pPool->FreeObject(this);
}