#pragma once
#include "libserver/common.h"
#include "libserver/socket_object.h"

class LoginObj : public NetworkIdentify {
public:
  LoginObj(NetworkIdentify* pIdentify, std::string account, std::string password);
  std::string GetAccount() const;
  std::string GetPassword() const;

private:
  std::string _password;
  std::string _account;
};