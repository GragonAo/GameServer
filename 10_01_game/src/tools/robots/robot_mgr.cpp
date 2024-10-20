#include "robot_mgr.h"
#include "libserver/common.h"
#include "libserver/global.h"
#include "libserver/yaml.h"
#include "libserver/entity_system.h"
#include "libserver/message_system_help.h"
#include "libserver/message_system.h"
#include "libserver/update_component.h"
#include "libserver/component_help.h"

#include "global_robots.h"
#include <sstream>

// 初始化 RobotMgr 对象，注册消息处理并设置定时任务
void RobotMgr::Awake()
{
    // 注册消息处理组件，监听 MI_RobotSyncState 消息
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();
    
    pMsgSystem->RegisterFunction(this, Proto::MsgId::MI_RobotSyncState, BindFunP1(this, &RobotMgr::HandleRobotState));

    // 添加一个定时器，每隔两秒调用 ShowInfo 方法展示机器人状态信息
    AddTimer(0, 2, false, 0, BindFunP0(this, &RobotMgr::ShowInfo));
}

// 重置 RobotMgr 的状态（返回到对象池）
void RobotMgr::BackToPool()
{
    // 清理资源或重置变量可以在这里处理
}

// 处理 MI_RobotSyncState 消息，更新机器人状态
void RobotMgr::HandleRobotState(Packet* pPacket)
{
    // 从数据包中解析 Proto::RobotSyncState 消息
    Proto::RobotSyncState protoState = pPacket->ParseToProto<Proto::RobotSyncState>();

    // 如果机器人列表为空且收到的状态大于0，表示测试开始
    if (_robots.empty() && protoState.states_size() > 0)
    {
        std::cout << "test begin" << std::endl;
        _start = std::chrono::system_clock::now();  // 记录测试开始时间
    }

    // 遍历每个机器人状态，更新 _robots 列表
    RobotStateType iType = RobotStateType::Space_EnterWorld;
    for (int index = 0; index < protoState.states_size(); index++)
    {
        auto proto = protoState.states(index);
        const auto account = proto.account();
        _robots[account] = RobotStateType(proto.state());
        
        // 记录机器人当前的最小状态
        if (_robots[account] < iType)
        {
            iType = _robots[account];
        }
    }

    _isChange = true;  // 标记状态发生变化
    NotifyServer(iType);  // 通知服务器当前状态
}

// 通知服务器所有机器人的状态
void RobotMgr::NotifyServer(RobotStateType iType)
{
    // 如果当前机器人数量不等于全局机器人数量，不做处理
    if (_robots.size() != GlobalRobots::GetInstance()->GetRobotsCount())
        return;

    // 查找是否有机器人状态小于当前状态 iType
    auto iter = std::find_if(_robots.begin(), _robots.end(), [&iType](auto pair)
        {
            return pair.second < iType;
        });

    // 如果所有机器人状态都不小于 iType，表示当前状态同步完成
    if (iter == _robots.end())
    {
        auto end = std::chrono::system_clock::now();  // 记录结束时间
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - _start);  // 计算持续时间
        auto stateName = GetRobotStateTypeShortName(iType);  // 获取状态名称
        std::cout << stateName << " over. time:" 
                  << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den 
                  << "s" << std::endl;  // 输出状态完成时间
    }
}

// 展示所有机器人的状态信息
void RobotMgr::ShowInfo()
{
    // 如果状态没有发生变化，跳过
    if (!_isChange)
        return;

    _isChange = false;  // 重置状态变化标志

    // 统计各个状态下的机器人数量
    std::map<RobotStateType, int> statData;
    std::for_each(_robots.cbegin(), _robots.cend(), [&statData](auto one)
        {
            auto state = one.second;
            if (statData.find(state) == statData.end())
            {
                statData[state] = 0;
            }
            ++statData[state];
        });

    // 构建输出信息
    std::stringstream show;
    auto curTime = timeutil::ToString(Global::GetInstance()->TimeTick);
    show << "++++++++++++++++++++++++++++ " << std::endl << curTime.c_str() << std::endl;

    // 按照状态类型遍历并输出每种状态下的机器人数量
    for (RobotStateType rss = RobotStateType::Http_Connecting; rss < RobotStateType::End; rss = static_cast<RobotStateType>((int)rss + 1))
    {
        if (statData.find(rss) == statData.end())
        {
            show << GetRobotStateTypeName(rss) << 0 << std::endl;
        }
        else
        {
            show << GetRobotStateTypeName(rss) << statData[rss] << std::endl;
        }
    }
    show << "++++++++++++++++++++++++++++" << std::endl;

    // 输出到控制台
    std::cout << show.str().c_str() << std::endl;
}
