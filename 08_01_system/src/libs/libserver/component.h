#pragma once

#include "common.h"
#include "sn_object.h"

class IEntity;
class SystemManager;
class IDynamicObjectPool;

class IComponent : virtual public SnObject {
public:
  friend class EntitySystem;

  virtual ~IComponent() = default;

  void SetPool(IDynamicObjectPool *pPool);
  void SetParent(IEntity *pObj);
  void SetSystemManager(SystemManager *pSys);

  bool IsActive() const { return _active; }

  template <class T> T *GetParent();

  SystemManager *GetSystemManager() const;
  virtual void BackToPool() = 0;
  virtual void ComponentBackToPool();

  virtual const char* GetTypeName() = 0;
  virtual uint64 GetTypeHashCode() = 0;

protected:
  bool _active{true};

private:
  IEntity *_parent{nullptr};
  SystemManager *_pSystemManager{nullptr};
  IDynamicObjectPool *_pPool{nullptr};
};

template <class T>
T* IComponent::GetParent()
{
    return dynamic_cast<T*>(_parent);
}

template <class T> class Component : public IComponent {
public:
  const char *GetTypeName();
  uint64 GetTypeHashCode();
};

template <class T> const char *Component<T>::GetTypeName() {
  return typeid(T).name();
}

template <class T> uint64 Component<T>::GetTypeHashCode() {
  return typeid(T).hash_code();
}