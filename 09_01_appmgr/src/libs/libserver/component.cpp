#include "component.h"
#include "entity.h"
#include "timer_component.h"
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

  if (!_timers.empty()) {
    auto pTimer =
        _pSystemManager->GetEntitySystem()->GetComponent<TimerComponent>();
    if (pTimer != nullptr)
      pTimer->Remove(_timers);

    _timers.clear();
  }

  if (_pPool != nullptr) {
    _pPool->FreeObject(this);
    _pPool = nullptr;
  }

  _sn = 0;
  _parent = nullptr;
  _pSystemManager = nullptr;
}

void IComponent::AddTimer(const int total, const int durations,
                          const bool immediateDo,
                          const int immediateDoDelaySecond,
                          TimerHandleFunction handler) {
  auto obj =
      GetSystemManager()->GetEntitySystem()->GetComponent<TimerComponent>();
  const auto timer =
      obj->Add(total, durations, immediateDo, immediateDoDelaySecond, handler);
  _timers.push_back(timer);
}