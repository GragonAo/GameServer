#include "create_component.h"              // 包含 CreateComponentC 类的声明
#include "common.h"                         // 包含通用定义
#include "component_factory.h"              // 包含组件工厂，用于创建组件
#include "packet.h"                         // 包含 Packet 类的定义
#include "entity.h"                         // 包含实体类的定义
#include "system_manager.h"                 // 包含系统管理器的定义
#include "message_system.h"
// 从参数元组中创建组件的辅助函数
template <typename... TArgs, size_t... Index>
IComponent* ComponentFactoryEx(EntitySystem* pEntitySystem, std::string className, const std::tuple<TArgs...>& args, std::index_sequence<Index...>)
{
    return pEntitySystem->AddComponentByName(className, std::get<Index>(args)...);
}

// 动态调用的模板结构
template<size_t ICount>
struct DynamicCall
{

    /*
       RepeatedPtrField 与 C++ 标准库中的 std::vector 类似，RepeatedPtrField 是一种动态大小的数组，专门设计用来存储 Protocol Buffers 消息的字段。
    */

    // 调用组件创建的通用方法
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        // 如果没有参数，直接创建组件
        if (params.size() == 0)
        {
            return ComponentFactoryEx(pEntitySystem, classname, t1, std::make_index_sequence<sizeof...(TArgs)>());
        }

        // 取出第一个参数并删除它
        Proto::CreateComponentParam param = (*(params.begin()));
        params.erase(params.begin());

        // 根据参数类型递归地构建参数元组并调用
        if (param.type() == Proto::CreateComponentParam::Int)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.int_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, t2, params);
        }

        if (param.type() == Proto::CreateComponentParam::String)
        {
            auto t2 = std::tuple_cat(t1, std::make_tuple(param.string_param()));
            return DynamicCall<ICount - 1>::Invoke(pEntitySystem, classname, t2, params);
        }

        return nullptr; // 如果参数类型不匹配，则返回 nullptr
    }
};

// 处理没有参数的情况
template<>
struct DynamicCall<0>
{
    template <typename... TArgs>
    static IComponent* Invoke(EntitySystem* pEntitySystem, const std::string classname, std::tuple<TArgs...> t1, google::protobuf::RepeatedPtrField<::Proto::CreateComponentParam>& params)
    {
        return nullptr; // 如果没有参数，则返回 nullptr
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Awake 方法用于初始化组件
void CreateComponentC::Awake()
{
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();

    // 注册消息处理函数
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_CreateComponent, BindFunP1(this, &CreateComponentC::HandleCreateComponent));
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_RemoveComponent, BindFunP1(this, &CreateComponentC::HandleRemoveComponent));
}

// BackToPool 方法用于将对象返回对象池（可重用）
void CreateComponentC::BackToPool()
{
    // 该方法目前为空，未实现具体逻辑
}

// 处理创建组件的逻辑
void CreateComponentC::HandleCreateComponent(Packet* pPacket) const
{
    // 解析数据包为 Proto::CreateComponent 消息
    Proto::CreateComponent proto = pPacket->ParseToProto<Proto::CreateComponent>();
    const std::string className = proto.class_name(); // 获取组件类名

    // 检查参数数量是否超过限制
    if (proto.params_size() >= 5)
    {
        std::cout << " !!!! CreateComponent failed. className:" << className.c_str() << " params size >= 5" << std::endl;
        return; // 返回，如果参数数量不符合
    }

    // 获取参数并调用动态调用方法创建组件
    auto params = proto.params();
    const auto pObj = DynamicCall<5>::Invoke(GetSystemManager()->GetEntitySystem(), className, std::make_tuple(), params);
    if (pObj == nullptr)
    {
        std::cout << " !!!! CreateComponent failed. className:" << className.c_str() << std::endl;
    }
    // std::cout << "CreateComponent. name:" << className << std::endl;
}

// 处理移除组件的逻辑
void CreateComponentC::HandleRemoveComponent(Packet* pPacket)
{
    Proto::RemoveComponent proto = pPacket->ParseToProto<Proto::RemoveComponent>();
    // 在这里可以添加逻辑以移除组件
    // GetEntitySystem()->RemoveFromSystem(proto.sn());
}
