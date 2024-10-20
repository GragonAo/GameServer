#pragma  once

#include "libserver/common.h"
#include "player.h"
#include "libserver/disposable.h"
#include <map>

class PlayerMgr : public IDisposable{
public:
    void AddPlayer(SOCKET socket,std::string account,std::string password);
    void RemovePlayer(SOCKET socket);
    Player* GetPlayer(SOCKET socket);
    Player* GetPlayer(std::string account);
    void Dispose() override;
private:
    std::map<SOCKET, Player*> _players;
    std::map<std::string,SOCKET> _accounts;
};