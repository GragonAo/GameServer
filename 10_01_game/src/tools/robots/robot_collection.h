#pragma once
#include "libserver/entity.h"
#include "libserver/system.h"
#include "robot.h"

// 机器人集合类，负责管理和操作机器人对象
class RobotCollection : public Entity<RobotCollection>, public IAwakeSystem<>
{
public:    
    // 唤醒方法，用于初始化该类的功能
    void Awake() override;
    
    // 对象回收到池中时的处理方法，用于清理资源
    void BackToPool() override;

    // 创建机器人实例，并将其添加到集合中
    void CreateRobot(std::string account);
    
    // 根据网络标识符找到相应的机器人对象
    Robot* GetMsgObj(NetworkIdentify* pIdentify);

protected:
    // 处理机器人连接网络的情况
    void HandleNetworkConnected(Robot* pRobot, Packet* pPacket);
    
    // 处理机器人断开网络的情况
    void HandleNetworkDisconnect(Robot* pRobot, Packet* pPacket);

    // 处理外部HTTP响应的情况
    void HandleHttpOuterResponse(Robot* pRobot, Packet* pPacket);

    // 处理账号检查的响应结果
    void HandleAccountCheckRs(Robot* pRobot, Packet* pPacket);

    // 处理服务器返回的玩家列表
    void HandlePlayerList(Robot* pRobot, Packet* pPacket);
    
    // 处理游戏令牌的返回结果
    void HandleGameToken(Robot* pRobot, Packet* pPacket);
    
    // 处理通过令牌登录的响应结果
    void HandleLoginByTokenRs(Robot* pRobot, Packet* pPacket);

private:
    // 存储机器人对象的集合，键为机器人账号，值为机器人指针
    std::map<std::string, Robot*> _robots;
};
