#pragma once

#include "system.h"

class UpdateSystem : virtual public  ISystem<UpdateSystem>
{
public:
    // 重写 ISystem 中的 Update 方法，用于更新所有的实体系统
    void Update(EntitySystem* pEntities) override;
};
