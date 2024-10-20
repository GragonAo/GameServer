#include "player_collector_component.h"
#include "libserver/common.h"
#include "libserver/socket_object.h"

// 组件初始化时调用的函数，当前无具体实现
void PlayerCollectorComponent::Awake() {}

// 当组件被放回对象池时调用，执行清理操作
void PlayerCollectorComponent::BackToPool() {
    auto pEntitySystem = GetSystemManager()->GetEntitySystem();
    auto iter = _players.begin();

    // 遍历并移除所有玩家组件
    while (iter != _players.end()) {
        pEntitySystem->RemoveComponent(iter->second); // 从实体系统中移除玩家组件
        ++iter;
    }

    // 清空账户和玩家映射
    _accounts.clear();
    _players.clear();
}

// 添加新玩家并返回其指针，如果添加失败返回 nullptr
Player *PlayerCollectorComponent::AddPlayer(NetworkIdentify *pIdentify, const std::string account) {
    const auto socket = pIdentify->GetSocketKey().Socket; // 获取玩家的 socket
    const auto iter = _players.find(socket);
    
    // 检查此 socket 是否已存在玩家
    if (iter != _players.end()) {
        std::cout << "AppPlayer error." << std::endl; // 若已存在，打印错误信息
        return nullptr;
    }

    // 将账户与 socket 映射
    _accounts[account] = socket;
    // 创建并添加玩家组件
    const auto pPlayer = GetSystemManager()->GetEntitySystem()->AddComponent<Player>(pIdentify, account);
    _players[socket] = pPlayer; // 将新玩家与 socket 关联
    return pPlayer; // 返回新玩家的指针
}

// 根据 socket 移除玩家
void PlayerCollectorComponent::RemovePlayerBySocket(SOCKET socket) {
    const auto iter = _players.find(socket); // 查找对应的玩家
    if (iter == _players.end())
        return; // 如果找不到则返回

    Player *pPlayer = iter->second; // 获取玩家对象
    _players.erase(socket); // 从 _players 中移除
    _accounts.erase(pPlayer->GetAccount()); // 从 _accounts 中移除对应的账户

    // 从实体系统中移除玩家组件
    GetSystemManager()->GetEntitySystem()->RemoveComponent(pPlayer);
}

// 根据玩家唯一标识符 (sn) 移除玩家
void PlayerCollectorComponent::RemovePlayerBySn(uint64 playerSn) {
    auto iter = std::find_if(_players.begin(), _players.end(), [&playerSn](auto pair) {
        return pair.second->GetPlayerSN() == playerSn; // 查找对应的玩家
    });

    if (iter == _players.end())
        return; // 找不到则返回

    // 通过 socket 移除玩家
    RemovePlayerBySocket(iter->second->GetSocketKey().Socket);
}

// 根据 socket 获取对应的玩家对象
Player *PlayerCollectorComponent::GetPlayerBySocket(const SOCKET socket) {
    const auto iter = _players.find(socket);
    if (iter == _players.end())
        return nullptr; // 找不到则返回 nullptr

    return iter->second; // 返回对应的玩家对象
}

// 根据账户名获取对应的玩家对象
Player *PlayerCollectorComponent::GetPlayerByAccount(const std::string account) {
    const auto iter = _accounts.find(account);
    if (iter == _accounts.end())
        return nullptr; // 找不到则返回 nullptr

    SOCKET socket = iter->second; // 获取对应的 socket
    auto iterPlayer = _players.find(socket);
    if (iterPlayer == _players.end()) {
        _accounts.erase(account); // 如果找不到玩家，移除账户
        return nullptr; // 返回 nullptr
    }
    return iterPlayer->second; // 返回玩家对象
}

// 根据玩家唯一标识符 (sn) 获取对应的玩家对象
Player *PlayerCollectorComponent::GetPlayerBySn(uint64 playerSn) {
    auto iter = std::find_if(_players.begin(), _players.end(), [&playerSn](auto pair) {
        return pair.second->GetPlayerSN() == playerSn; // 查找对应的玩家
    });

    if (iter == _players.end())
        return nullptr; // 找不到则返回 nullptr

    return iter->second; // 返回玩家对象
}

// 返回当前在线玩家的数量
int PlayerCollectorComponent::OnlineSize() const { return _players.size(); }

// 获取所有在线玩家的映射
std::map<SOCKET, Player *> &PlayerCollectorComponent::GetAll() {
    return _players; // 返回玩家集合
}
