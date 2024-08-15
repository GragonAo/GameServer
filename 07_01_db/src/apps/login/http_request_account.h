#pragma  once
#include "http_request.h"
#include "libserver/system.h"
#include <string>

class HttpRequestAccount : public HttpRequest,public IAwakeFromPoolSystem<std::string,std::string>{
    public:
        void AwakeFromPool(std::string account, std::string password) override;
    protected:
        void ProcessMsg(Json::Value value) override;
    private:
        std::string _password;
};