#include "entity.h"
#include "component.h"
#include "entity_system.h"

void IEntity::ComponentBackToPool() {
  auto pSystemManager = GetSystemManager();
  for (auto pair : _components) {
    if (pSystemManager != nullptr)
      pSystemManager->GetEntitySystem()->RemoveComponent(pair.second);
    else
      pair.second->ComponentBackToPool();
  }

  _components.clear();

  IComponent::ComponentBackToPool();
}

void IEntity::RemoveComponent(IComponent *pComponent) {
  const auto typeHashCode = pComponent->GetTypeHashCode();
  _components.erase(typeHashCode);

  const auto pSystemManager = GetSystemManager();
  if (pSystemManager == nullptr) {
    pComponent->ComponentBackToPool();
  } else {
    pSystemManager->GetEntitySystem()->RemoveComponent(pComponent);
  }
}