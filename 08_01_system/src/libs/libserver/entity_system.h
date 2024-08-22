#pragma once

#include "cache_swap.h"
#include "component.h"
#include "component_collections.h"
#include "component_factory.h"
#include "disposable.h"
#include "global.h"
#include "log4_help.h"
#include "object_pool.h"
#include "object_pool_collector.h"
#include "system_manager.h"

class Packet;

class EntitySystem : public IDisposable {
public:
  friend class ConsoleThreadComponent;

  EntitySystem(SystemManager *pMgr);
  virtual ~EntitySystem();

  template <class T, typename... TArgs> T *AddComponent(TArgs... args);

  template <typename... TArgs>
  IComponent *AddComponentByName(std::string className, TArgs... args);

  template <class T> T *GetComponent();

  void RemoveComponent(IComponent *pObj);

  template <class T> ComponentCollections *GetComponentCollections();

  void Update();
  void Dispose() override;
  std::map<uint64, ComponentCollections *> GetObjgSystem() const;

private:
  template <class T> void AddComponent(T *pComponent);

private:
  std::map<uint64, ComponentCollections *> _objSystems;
  SystemManager *_systemManager;
};

template <class T> inline void EntitySystem::AddComponent(T *pComponent) {
  const auto typeHashCode = pComponent->GetTypeHashCode();

  auto iter = _objSystems.find(typeHashCode);
  if (iter == _objSystems.end()) {
    _objSystems[typeHashCode] =
        new ComponentCollections(pComponent->GetTypeName());
  }
  auto pEntities = _objSystems[typeHashCode];
  pEntities->Add(dynamic_cast<IComponent *>(pComponent));
}

template <class T, typename... TArgs>
T *EntitySystem::AddComponent(TArgs... args) {
  auto pCollector = _systemManager->GetPoolCollector();
  auto pPool = (DynamicObjectPool<T> *)pCollector->GetPool<T>();
  auto pComponent =
      pPool->MallocObject(_systemManager, std::forward<TArgs>(args)...);

  if (pComponent == nullptr)
    return nullptr;
  AddComponent(pComponent);
  return pComponent;
}

template <typename... TArgs>
inline IComponent *EntitySystem::AddComponentByName(std::string className,
                                                    TArgs... args) {
  auto pComponent = ComponentFactory<TArgs...>::GetInstance()->Create(
      _systemManager, className, std::forward<TArgs>(args)...);
  if (pComponent == nullptr)
    return nullptr;
  AddComponent(pComponent);
  return pComponent;
}

template <class T> T *EntitySystem::GetComponent() {
  const auto typeHashCode = typeid(T).hash_code();
  auto iter = _objSystems.find(typeHashCode);
  if (iter == _objSystems.end())
    return nullptr;
  return dynamic_cast<T *>(iter->second->Get());
}

template <class T>
inline ComponentCollections *EntitySystem::GetComponentCollections() {
  const auto typeHashCode = typeid(T).hash_code();
  auto iter = _objSystems.find(typeHashCode);
  if (iter == _objSystems.end()) {
    return nullptr;
  }
  return iter->second;
}