#include "robot_console_http.h"
#include "libserver/thread_mgr.h"
#include "test_http_login.h"

// 注册命令处理函数
void RobotConsoleHttp::RegisterHandler()
{
    // 注册"-check"命令，当用户输入该命令时，调用HandleRequest函数
    OnRegisterHandler("-check", BindFunP1(this, &RobotConsoleHttp::HandleRequest));
}

// 输出帮助信息
void RobotConsoleHttp::HandleHelp()
{
    // 输出命令提示信息，解释如何使用"-check"命令
    std::cout << "\t-check test password.\t\tcheck account." << std::endl;
}

// 处理"-check"命令的逻辑
// params：存储了命令行参数（账号和密码）
void RobotConsoleHttp::HandleRequest(std::vector<std::string>& params)
{
    // 检查参数数量是否正确，这里需要两个参数：账号和密码
    if (!CheckParamCnt(params, 2))
        return;
    
    // 使用ThreadMgr管理线程，创建一个TestHttpLogin组件来执行登录操作
    // 传入的参数是用户输入的账号和密码
    ThreadMgr::GetInstance()->CreateComponent<TestHttpLogin>(params[0], params[1]);
}
