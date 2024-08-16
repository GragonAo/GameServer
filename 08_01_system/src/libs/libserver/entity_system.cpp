#include "entity_system.h"
#include "common.h"
#include "component.h"
#include "console.h"
#include "log4.h"
#include "log4_help.h"
#include "message_system.h"
#include "system_manager.h"

// 构造函数：初始化EntitySystem对象，传入的SystemManager指针保存在成员变量_systemManager中
EntitySystem::EntitySystem(SystemManager *pMgr) {
  _systemManager = pMgr;  // 将传入的系统管理器指针保存在私有成员变量中
}

// 析构函数：默认的析构函数，当前类没有需要手动释放的资源
EntitySystem::~EntitySystem() {}

// 删除组件的方法：从系统中移除传入的组件对象
void EntitySystem::RemoveComponent(IComponent *pObj) {
  const auto entitySn = pObj->GetSN();  // 获取组件所属实体的唯一标识符
  const auto typeHashCode = pObj->GetTypeHashCode();  // 获取组件类型的哈希码

  // 查找是否存在管理该类型组件的系统
  auto iterObj = _objSystems.find(typeHashCode);
  if (iterObj == _objSystems.end()) {  // 如果找不到相关系统
    LOG_WARN("destroy class failed. class's name: " << pObj->GetTypeName()
                                                    << " . not found class.");
    return;  // 记录警告日志，说明未找到对应的组件类，函数返回
  }

  // 从找到的系统中移除该组件
  iterObj->second->Remove(entitySn);
}

// 更新系统的方法：对所有组件系统进行更新操作
void EntitySystem::Update() {
  for (auto iter : _objSystems) {  // 遍历所有的组件系统
    iter.second->Swap();  // 执行Swap操作，通常用于双缓冲处理，将更新后的数据交换到前台
  }
}

// 释放系统资源的方法：销毁不在baseClass列表中的系统
void EntitySystem::Dispose() {
  std::set<uint64> baseClass;  // 创建一个set用于存储不应销毁的基础类的类型哈希码
  baseClass.insert(typeid(Log4).hash_code());  // 将Log4类的哈希码加入集合
  baseClass.insert(typeid(Console).hash_code());  // 将Console类的哈希码加入集合

  for (auto iter : _objSystems) {  // 遍历所有的组件系统
    if (baseClass.find(iter.first) != baseClass.end())  // 如果该系统属于基础类，则跳过销毁
      continue;
    iter.second->Dispose();  // 调用系统的Dispose方法，释放资源
    delete iter.second;  // 删除该系统对象，释放内存
  }

  _objSystems.clear();  // 清空系统容器，确保所有指针都无效化
}
