#include "console_cmd_app.h"  // 包含 ConsoleCmdApp 类的头文件
#include "libserver/message_system_help.h"  // 包含消息系统帮助函数的头文件

// 注册命令处理器
void ConsoleCmdApp::RegisterHandler()
{
    // 注册 "-info" 命令，绑定命令处理函数 HandleAppInfo
    OnRegisterHandler("-info", BindFunP1(this, &ConsoleCmdApp::HandleAppInfo));
}

// 显示帮助信息
void ConsoleCmdApp::HandleHelp()
{
    // 输出关于 "-info" 命令的帮助信息
    std::cout << "\t-info.\t\tshow information of app." << std::endl;
}

// 处理应用信息的命令
void ConsoleCmdApp::HandleAppInfo(std::vector<std::string>& params)
{
    // 创建一个 CmdApp 协议对象
    Proto::CmdApp cmdProto;
    
    // 设置命令类型为信息查询
    cmdProto.set_cmd_type(Proto::CmdApp_CmdType_Info);
    
    // 通过消息系统帮助类发送命令包，通知其他组件处理此命令
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_CmdApp, cmdProto, nullptr);
}
