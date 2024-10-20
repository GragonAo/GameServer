#pragma once

#include "libserver/system.h"   // 系统基类，提供基础系统功能
#include "libserver/entity.h"   // 实体基类，提供实体管理功能

// WorldComponentGather 类用于收集和同步世界相关信息
class WorldComponentGather : public Entity<WorldComponentGather>, public IAwakeSystem<>
{
public:
    // 在对象创建时调用，用于初始化该组件
    void Awake() override;
    
    // 在对象回收到对象池时调用，用于资源清理
    void BackToPool() override;

private:
    // 用于将当前世界的信息同步到聚合服务器
    void SyncWorldInfoToGather() const;
};
