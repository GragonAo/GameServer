#include "component_collections.h"
#include "log4_help.h"

ComponentCollections::ComponentCollections(std::string componentName) {
  _componentName = componentName;
}

ComponentCollections::~ComponentCollections()
{
    if (!_objs.empty() || !_addObjs.empty())
    {
        std::cout << " class:" << _componentName.c_str() << " still reachable. " << std::endl;
    }

    _removeObjs.clear();
}

void ComponentCollections::Add(IComponent *pObj) {

  //不添加重复的组件
  if (_objs.find(pObj->GetSN()) != _objs.end() ||
      _addObjs.find(pObj->GetSN()) != _addObjs.end()) {

    LOG_ERROR("add component error. Repeat SN. type: "
              << pObj->GetTypeName() << " sn: " << pObj->GetSN()
              << " type: " << this->GetClassType().c_str());

    return;
  }
  _addObjs[pObj->GetSN()] = pObj;
}

//根据组件sn标识获取组件
IComponent *ComponentCollections::Get(const uint64 sn) {
  if (sn == 0) {
    if (!_objs.empty())
      return _objs.begin()->second;
    if (!_addObjs.empty())
      return _addObjs.begin()->second;
  } else {

    const auto iter1 = _objs.find(sn);
    if (iter1 != _objs.end())
      return iter1->second;

    const auto iter2 = _addObjs.find(sn);
    if (iter2 != _addObjs.end())
      return iter2->second;
  }
  return nullptr;
}

//移除组件
void ComponentCollections::Remove(uint64 sn) { _removeObjs.push_back(sn); }

//获取集合中的全部组件
std::map<uint64, IComponent *> &ComponentCollections::GetAll() { return _objs; }

//交换，目的是将添加和移除进行分帧处理
void ComponentCollections::Swap() {
  if (!_addObjs.empty()) {
    for (auto pair : _addObjs) {
      _objs.insert(std::make_pair(pair.first, pair.second));
    }
    _addObjs.clear();
  }
  
  if (!_removeObjs.empty()) {
    for (auto sn : _removeObjs) {
      const auto iter = _objs.find(sn);
      if (iter != _objs.end()) {
        iter->second->ComponentBackToPool();
        _objs.erase(iter);
        continue;
      }
      LOG_WARN("remove obj failed. sn: " << sn << " type: "
                                         << this->GetClassType().c_str());
    }
    _removeObjs.clear();
  }
}

//销毁
void ComponentCollections::Dispose() {
  for (const auto pair : _objs) {
    auto pComponent = pair.second;
    if (pComponent->GetSN() == 0)
      continue;
    pComponent->ComponentBackToPool();
  }
  _objs.clear();
  for (const auto pair : _addObjs) {
    auto pComponent = pair.second;
    if (pComponent->GetSN() == 0)
      continue;
    pComponent->ComponentBackToPool();
  }
  _addObjs.clear();
  _removeObjs.clear();
}

//获取组件集合的名称
std::string ComponentCollections::GetClassType() const {
  return _componentName;
}