#include "test_http_login.h"
#include "libserver/common.h"
#include "libserver/component_help.h"
#include "libserver/log4_help.h"
#include "libserver/message_callback.h"
#include "libserver/message_component.h"
#include "libserver/message_system_help.h"
#include "libserver/network_type.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/socket_object.h"
#include "libserver/yaml.h"

void TestHttpLogin::Awake(std::string account, std::string password) {
  _account = account;
  _password = password;

  _objKey = {ObjectKeyType::Account, {0, _account}};

  auto pMsgCallBack = new MessageCallBackFunctionFilterObj<TestHttpLogin>();
  AddComponent<MessageComponent>(pMsgCallBack);
  pMsgCallBack->GetPacketObject = [this](NetworkIdentify *pIdentify) {
    if (_objKey == pIdentify->GetObjectKey())
      return this;
    return static_cast<TestHttpLogin *>(nullptr);
  };

  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::MI_NetworkConnected,
      BindFunP2(this, &TestHttpLogin::HandleNetworkConnected));
  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::MI_NetworkDisconnect,
      BindFunP2(this, &TestHttpLogin::HandleNetworkDisconnect));
  pMsgCallBack->RegisterFunctionWithObj(
      Proto::MsgId::MI_HttpOuterResponse,
      BindFunP2(this, &TestHttpLogin::HandleHttpOuterResponse));

  auto pYaml = ComponentHelp::GetYaml();
  const auto pLoginConfig = dynamic_cast<LoginConfig *>(pYaml);
  ParseUrlInfo info;
  if (!MessageSystemHelp::ParseUrl(pLoginConfig->UrlLogin, info)) {
    LOG_ERROR("parse login url failed. url: " << pLoginConfig->UrlLogin);
    return;
  }

  _ip = info.Host;
  _port = info.Port;
  _mothed = info.Mothed;

  Proto::NetworkConnect protoConn;
  protoConn.set_network_type((int)NetworkType::HttpConnector);
  _objKey.SerializeToProto(protoConn.mutable_key());
  protoConn.set_ip(_ip.c_str());
  protoConn.set_port(_port);
  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkConnect, protoConn,
                                    nullptr);
}

void TestHttpLogin::BackToPool() {}

void TestHttpLogin::HandleNetworkDisconnect(TestHttpLogin *pObj,
                                            Packet *pPacket) {
  LOG_ERROR("test http login. recv network disconnect.");
}
void TestHttpLogin::HandleNetworkConnected(TestHttpLogin *pObj,
                                           Packet *pPacket) {
  _socketKey = pPacket->GetSocketKey();
  LOG_DEBUG("conneted. " << pPacket);
  std::map<std::string, std::string> params;
  params["account"] = _account;
  params["password"] = _password;
  MessageSystemHelp::SendHttpRequest(this, _ip, _port, _mothed, &params);
}
void TestHttpLogin::HandleHttpOuterResponse(TestHttpLogin *pObj,
                                            Packet *pPacket) {
  auto protoHttp = pPacket->ParseToProto<Proto::Http>();
  LOG_DEBUG("http code: " << protoHttp.status_code());
  LOG_DEBUG(protoHttp.body().c_str());
}