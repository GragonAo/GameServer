#pragma once
#include "component.h"
#include "system.h"
#include <functional>

class UpdateComponent :public Component<UpdateComponent>, public IAwakeFromPoolSystem<>
{
public:
    // 组件从池中唤醒时调用
    void Awake() override;

    // 组件回收时调用
    void BackToPool() override;

    // 用于存储更新函数的可调用对象
    std::function<void()> UpdataFunction{ nullptr };    
};
