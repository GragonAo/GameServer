#include "update_system.h"
#include "entity.h"
#include "update_component.h"

void UpdateSystem::Update(EntitySystem* pEntities)
{
    // 获取实体系统中所有的 UpdateComponent 组件集合
    auto pCollections = pEntities->GetComponentCollections<UpdateComponent>();
    
    // 如果没有找到组件集合，则返回
    if (pCollections == nullptr)
        return;

    // 交换缓存，用于多线程安全的更新机制
    pCollections->Swap();

    // 获取所有组件的列表
    auto lists = pCollections->GetAll();
    
    // 遍历每个组件并调用它的更新函数
    for (const auto one : lists)
    {
        const auto pComponent = one.second;
        const auto pUpdateComponent = static_cast<UpdateComponent*>(pComponent);
        pUpdateComponent->Update();
    }
}
