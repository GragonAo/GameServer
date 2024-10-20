#pragma once

#include "common.h"
#include "component.h"
#include "log4_help.h"
#include "object_pool.h"
#include <algorithm>
#include <cstddef>
#include "entity_system.h"

class IEntity : public IComponent {
public:
  virtual ~IEntity() = default;
  void ComponentBackToPool() override;

  template <class T, typename... TArgs> T *AddComponent(TArgs... args);

  template <class T> T *GetComponent();

  template <class T> void RemoveComponent();

  void RemoveComponent(IComponent *pObj);

protected:
  std::map<uint64, IComponent *> _components;
};

template <class T, typename... TArgs>
inline T *IEntity::AddComponent(TArgs... args) {
  const auto typeHashCode = typeid(T).hash_code();
  if(_components.find(typeHashCode) != _components.end()){
    LOG_ERROR("Add same component. type: " << typeid(T).name());
    return nullptr;
  }
  
    auto pComponent = GetSystemManager()->GetEntitySystem()->AddComponent<T>(std::forward<TArgs>(args)...);
    pComponent->SetParent(this);
    _components.insert(std::make_pair(pComponent->GetSN(), pComponent));
    return pComponent;
}

template <class T> T *IEntity::GetComponent() {
  const auto typeHashCode = typeid(T).hash_code();
  const auto iter = _components.find(typeHashCode);
  if (iter == _components.end())
    return nullptr;
  return dynamic_cast<T *>(iter->second);
}

template <class T> void IEntity::RemoveComponent() {
  const auto typeHashCode = typeid(T).hash_code();
  const auto iter = _components.find(typeHashCode);
  if (iter == _components.end()) {
    LOG_ERROR("Entity RemoveComponent error. not find. className: "
              << typeid(T).name());
    return;
  }
  auto pComponent = iter->second;
  RemoveComponent(pComponent);
}

template <class T> class Entity : public IEntity {
public:
  virtual const char *GetTypeName();
  uint64 GetTypeHashCode();
};

template <class T> const char *Entity<T>::GetTypeName() {
  return typeid(T).name();
}

template <class T> uint64 Entity<T>::GetTypeHashCode() {
  return typeid(T).hash_code();
}