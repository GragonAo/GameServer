#include "login_obj.h"
#include "libserver/message_system_help.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/socket_object.h"

LoginObj::LoginObj(NetworkIdentify *pIdentify, std::string account,
                   std::string password) {
  _account = account;
  _password = password;

  _socketKey = pIdentify->GetSocketKey();
  _objKey = {ObjectKeyType::Account, ObjectKeyValue{0, _account}};
  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkListenKey, this);
}

std::string LoginObj::GetAccount() const { return _account; }

std::string LoginObj::GetPassword() const { return _password; }