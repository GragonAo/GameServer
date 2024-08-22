#include "system_manager.h"
#include "console_thread_component.h"
#include "create_component.h"
#include "entity_system.h"
#include "object_pool_collector.h"
#include "update_system.h"
#include <random>
#include <sstream>
#include <thread>

SystemManager::SystemManager() {
  _pEntitySystem = new EntitySystem(this);
  _pMessageSystem = new MessageSystem(this);

  _systems.emplace_back(new UpdateSystem());

  std::stringstream strStream;
  strStream << std::this_thread::get_id();
  std::string idstr = strStream.str();
  std::seed_seq seed1(idstr.begin(), idstr.end());
  std::minstd_rand0 generator(seed1);
  _pRandomEngine = new std::default_random_engine(generator);

  _pPoolCollector = new DynamicObjectPoolCollector(this);
}

void SystemManager::InitComponent(ThreadType threadType) {
  _pEntitySystem->AddComponent<CreateComponentC>();
  _pEntitySystem->AddComponent<ConsoleThreadComponent>(threadType);
}

void SystemManager::Update() {
  _pPoolCollector->Update();
  
  _pEntitySystem->Update();
  _pMessageSystem->Update(_pEntitySystem);

  for (auto iter = _systems.begin(); iter != _systems.end(); ++iter) {
    (*iter)->Update(_pEntitySystem);
  }
}

void SystemManager::Dispose() {

  for (auto one : _systems) {
    one->Dispose();
    delete one;
  }
  _systems.clear();

  delete _pRandomEngine;
  _pRandomEngine = nullptr;

  _pMessageSystem->Dispose();
  delete _pMessageSystem;
  _pMessageSystem = nullptr;

  _pEntitySystem->Dispose();
  delete _pEntitySystem;
  _pEntitySystem = nullptr;
}

MessageSystem *SystemManager::GetMessageSystem() const {
  return _pMessageSystem;
}
EntitySystem *SystemManager::GetEntitySystem() const { return _pEntitySystem; }

std::default_random_engine *SystemManager::GetRandomEngine() const {
  return _pRandomEngine;
}

DynamicObjectPoolCollector *SystemManager::GetPoolCollector() const {
  return _pPoolCollector;
}