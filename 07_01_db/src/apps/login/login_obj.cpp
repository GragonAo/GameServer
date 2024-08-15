#include "login_obj.h"

LoginObj::LoginObj(SOCKET socketInfo, std::string account,
                   std::string password) {
  _socket = socketInfo;
  _account = account;
  _password = password;
}
std::string LoginObj::GetAccount() const { return _account; }
SOCKET LoginObj::GetSocket() { return _socket; }