#include "http_request_account.h"
#include "http_request.h"
#include "libserver/app_type.h"
#include "libserver/component_help.h"
#include "libserver/message_system_help.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/update_component.h"
#include "libserver/yaml.h"

void HttpRequestAccount::Awake(std::string account, std::string password) {

  // update
  auto pUpdateComponent = AddComponent<UpdateComponent>();
  pUpdateComponent->UpdateFunction = BindFunP0(this, &HttpRequest::Update);

  _account = account;
  _password = password;
  _curlRs = CRS_None;
  _method = HttpResquestMethod::HRM_Post;
  _state = HttpResquestState::HRS_Send;

  auto pYaml = ComponentHelp::GetYaml();
  const auto pLoginConfig =
      dynamic_cast<LoginConfig *>(pYaml->GetConfig(APP_LOGIN));

  _url = pLoginConfig->UrlLogin;

  // 创建 JSON 对象
  Json::Value jsonData;
  jsonData["account"] = _account;
  jsonData["password"] = _password;

  // 将 JSON 对象转换为字符串
  Json::StreamWriterBuilder writerBuilder;
  _params = Json::writeString(writerBuilder, jsonData);
}
void HttpRequestAccount::ProcessMsg(Json::Value value) {
  Proto::AccountCheckReturnCode code =
      Proto::AccountCheckReturnCode::ARC_UNKONWN;
  int httpcode = value["code"].asInt();
  if (httpcode == 0)
    code = Proto::AccountCheckReturnCode::ARC_OK;
  else if (httpcode == 2)
    code = Proto::AccountCheckReturnCode::ARC_NOT_FOUND_ACCOUNT;
  else if (httpcode == 3)
    code = Proto::AccountCheckReturnCode::ARC_PASSWORD_WRONG;
  Proto::AccountCheckToHttpRs checkProto;
  checkProto.set_account(_account);
  checkProto.set_return_code(code);

  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_AccountCheckToHttpRs, 0,
                                    checkProto);
}