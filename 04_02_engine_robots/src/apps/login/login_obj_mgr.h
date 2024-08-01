#pragma once

#include "libserver/common.h"
#include "libserver/disposable.h"
#include "login_obj.h"
#include <map>

class LoginObjMgr : public IDisposable {
public:
  void AddPlayer(SOCKET socket, std::string account, std::string password);
  void RemovePlayer(SOCKET socket);
  LoginObj *GetPlayer(SOCKET socket);
  LoginObj *GetPlayer(std::string account);
  void Dispose() override;

private:
  std::map<SOCKET, LoginObj *> _players;
  std::map<std::string, SOCKET> _accounts;
};