#include "http_request_account.h"
#include "http_request.h"
#include "libserver/app_type_mgr.h"
#include "libserver/message_system.h"
#include "libserver/packet.h"
#include "libserver/yaml.h"

void HttpRequestAccount::AwakeFromPool(std::string account, std::string password)
{
    _account = account;
    _password = password;
    _curlRs = CRS_None;
    _method = HttpResquestMethod::HRM_Post;

    auto pYaml = Yaml::GetInstance();
    const auto pLoginConfig = dynamic_cast<LoginConfig*>(pYaml->GetConfig(APP_LOGIN));

    // _url = "http://192.168.1.134:3001/account/login";
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
    Proto::AccountCheckRs::ReturnCode code  = Proto::AccountCheckRs::ARC_UNKONWN;
    int httpcode = value["code"].asInt();
    if(httpcode == 0)
        code = Proto::AccountCheckRs::ARC_OK;
    else if(httpcode == 2)
        code = Proto::AccountCheckRs::ARC_NOT_FOUND_ACCOUNT;
    else if(httpcode == 3)
        code = Proto::AccountCheckRs::ARC_PASSWORD_WRONG;
    Proto::AccountCheckToHttpRs checkProto;
    checkProto.set_account(_account);
    checkProto.set_return_code(code);
    auto pCheckPacket = new Packet(Proto::MsgId::MI_AccountCheckToHttpRs,0);
    pCheckPacket->SerializeToBuffer(checkProto);
    IMessageSystem::DispatchPacket(pCheckPacket);
}