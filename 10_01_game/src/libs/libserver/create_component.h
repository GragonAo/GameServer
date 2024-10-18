#pragma once

#include "entity.h"          // 引入实体基类
#include "system.h"         // 引入系统基类
#include "message_system.h" // 引入消息系统，用于处理消息和数据包

class Packet; // 前向声明 Packet 类

// CreateComponentC 类用于处理组件的创建和移除，继承自 Entity 和 IAwakeSystem
class CreateComponentC : public Entity<CreateComponentC>, public IAwakeSystem<> {
public:
    // Awake 方法用于在对象激活时执行初始化操作
    void Awake() override;

    // BackToPool 方法用于将对象返回对象池，以便重用
    void BackToPool() override;

private:
    // HandleCreateComponent 方法用于处理组件创建的消息
    void HandleCreateComponent(Packet* pPacket) const;

    // HandleRemoveComponent 方法用于处理组件移除的消息
    void HandleRemoveComponent(Packet* pPacket);
};
