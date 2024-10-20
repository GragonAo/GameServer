#include "component.h"
#include "entity.h"
#include "timer_component.h"
#include "system_manager.h"

//设置当前组件的父组件
void IComponent::SetParent(IEntity *pObj) { _parent = pObj; }

//设置归属于那个对象池
void IComponent::SetPool(IDynamicObjectPool *pPool) { _pPool = pPool; }

//设置归属于那个系统管理类
void IComponent::SetSystemManager(SystemManager *pSys) {
  _pSystemManager = pSys;
}

//获取当前组件的父组件
IEntity *IComponent::GetParent() const { return _parent; }

//获取系统管理类
SystemManager *IComponent::GetSystemManager() const { return _pSystemManager; }

//组件回收
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

//添加计时任务
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