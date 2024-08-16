#include "entity.h"
#include "entity_system.h"
#include "component.h"

void IEntity::BackToPool() {
  for (const auto &one : _components) {
    const auto pEntitySystem = GetSystemManager()->GetEntitySystem();
    if (pEntitySystem != nullptr)
      pEntitySystem->RemoveComponent(one.second);
    else
      one.second->ComponentBackToPool();
  }
  _components.clear();
}

void IEntity::RemoveComponent(IComponent *pComponent) {
  const auto typeHashCode = pComponent->GetTypeHashCode();
  _components.erase(typeHashCode);

  const auto pEntitySystem = GetSystemManager()->GetEntitySystem();
  if (pEntitySystem == nullptr) {
    pComponent->ComponentBackToPool();
  } else {
    pEntitySystem->RemoveComponent(pComponent);
  }
}