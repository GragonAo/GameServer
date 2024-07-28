#include "player.h"

Player::Player(SOCKET socketInfo, std::string account, std::string password) {
  _socket = socketInfo;
  _account = account;
  _password = password;
}
std::string Player::GetAccount() const { return _account; }
SOCKET Player::GetSocket() { return _socket; }