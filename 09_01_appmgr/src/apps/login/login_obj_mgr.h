#pragma once

#include "libserver/common.h"
#include "libserver/component.h"
#include "libserver/disposable.h"
#include "login_obj.h"
#include <map>

class LoginObjMgr : public Component<LoginObjMgr> {
public:
  void AddPlayer(SOCKET socket, std::string account, std::string password);
  void RemovePlayer(SOCKET socket);
  LoginObj *GetPlayer(SOCKET socket);
  LoginObj *GetPlayer(std::string account);
  size_t Count() const;

  void BackToPool() override;

private:
  std::map<SOCKET, LoginObj *> _players;
  std::map<std::string, SOCKET> _accounts;
};