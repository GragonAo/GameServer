#include "component_help.h"
#include "trace_component.h"
#include "thread_mgr.h"

//以下的组件均在主线程中

//获取全局的实例系统
EntitySystem* ComponentHelp::GetGlobalEntitySystem(){
    return ThreadMgr::GetInstance()->GetEntitySystem();
}

//获取全局的Yaml加载组件
Yaml* ComponentHelp::GetYaml(){
    return ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<Yaml>();
}

//获取全局的资源的路径加载器
ResPath* ComponentHelp::GetResPath(){
    return ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<ResPath>();
}

//获取追溯组件，用于追溯详细的信息等
TraceComponent* ComponentHelp::GetTraceComponent(){
    return ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<TraceComponent>();
}
