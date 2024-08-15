#pragma once

#include "cache_swap.h"
#include "component.h"
#include "component_factory.h"
#include "disposable.h"
#include "object_pool_interface.h"
#include "message_system.h"
#include "object_pool.h"
#include "sn_object.h"
#include "system.h"
#include <mutex>
#include <utility>
class Packet;
class IEntity;

class EntitySystem : virtual public SnObject, public IDisposable {
public:
  virtual ~EntitySystem();

  // friend
  // 关键字在C++中用于允许非成员函数或其他类的成员函数访问类的私有和受保护成员

  friend class IEntity;
  void InitComponent();

  template <class T, typename... TArgs> T *AddComponent(TArgs... args);

  template <typename... TArgs>
  IComponent *AddComponentByName(std::string className, TArgs... args);

  template <class T> T *GetComponent();

  virtual void Update();
  void UpdateMessage();
  void Dispose() override;
  void AddPacketToList(Packet *pPacket);

protected:
    void AddToSystem(IComponent* pObj);

protected:
  std::list<IUpdateSystem *> _updateSystems;
  std::list<IMessageSystem *> _messageSystem;
  std::map<uint64, IComponent *> _objSystems;
  std::mutex _packet_lock;
  CacheSwap<Packet> _cachePakcet;
};

template <class T, typename... TArgs>
T *EntitySystem::AddComponent(TArgs... args) {
  auto pComponent = DynamicObjectPool<T>::GetInstance()->MallocObject(
      std::forward<TArgs>(args)...);
  AddToSystem(pComponent);
  return pComponent;
}

template <typename... TArgs>
inline IComponent *EntitySystem::AddComponentByName(std::string className,
                                                    TArgs... args) {
  auto pComponent = ComponentFactory<TArgs...>::GetInstance()->Create(
      className, std::forward<TArgs>(args)...);
  if (pComponent == nullptr)
    return nullptr;
  AddToSystem(pComponent);
  return pComponent;
}

template <class T> T *EntitySystem::GetComponent() {
  auto iter =
      std::find_if(_objSystems.begin(), _objSystems.end(), [](auto pair) {
        if (dynamic_cast<T *>(pair.second) != nullptr) {
          return true;
        }
        return false;
      });
  if (iter == _objSystems.end())
    return nullptr;
  return dynamic_cast<T *>(iter->second);
}