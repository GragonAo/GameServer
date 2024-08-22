#include "res_path.h"           // 包含ResPath类的头文件
#include <linux/limits.h>       // 包含定义Linux系统中的路径长度常量，例如PATH_MAX
#include <ostream>              // 包含输出流库
#include <unistd.h>             // 包含POSIX API，如readlink函数
#include "iostream"             // 包含输入输出流库，用于标准输出
#include "util_string.h"  // 包含字符串处理的自定义库

// 构造函数：初始化资源路径
void ResPath::Awake() {
    // 尝试从环境变量 "GENGINE_RES_PATH" 获取路径
    char* envValue = getenv("GENGINE_RES_PATH");
    _resPath = envValue == nullptr? "" : envValue;

    // 如果环境变量为空，自动推断可执行文件的路径
    if(_resPath.empty()){
        char name[PATH_MAX]; // 用于存储可执行文件路径的字符数组，大小为系统定义的最大路径长度
        int rslt = ::readlink("/proc/self/exe",name,PATH_MAX); // 读取当前可执行文件的路径

        // 检查readlink的返回值，如果失败或者路径长度超出限制，打印错误并返回
        if(rslt < 0 || rslt >= PATH_MAX){
            std::cout<<"ResPath::Initialize. res path failed." << std::endl;
            return;
        }

        name[rslt] = 0; // 将路径字符串终止
        _resPath = name; // 将读取到的路径赋值给_resPath

        // 截断路径两次，以获取上级目录
        for(int i = 0; i < 2; i++){
            const auto index = _resPath.find_last_of("/");
            _resPath = _resPath.substr(0, index); // 获取最后一个斜杠之前的部分
        }
    }

    _resPath += "/res"; // 在路径末尾加上 "/res" 子目录
    std::cout << "GENGINE_RES_PATH=" << _resPath << std::endl; // 打印最终的资源路径
}

void ResPath::BackToPool()
{
}

// 查找资源路径：将给定的资源文件名拼接到资源路径上
std::string ResPath::FindResPath(const std::string &res) {
    return FindResPath(res.c_str());
}

// 查找资源路径的实现，接受C风格字符串
std::string ResPath::FindResPath(const char *res) {
    auto fpath = _resPath + res; // 拼接资源路径和资源名

    // 使用自定义的字符串替换函数，将反斜杠替换为正斜杠，将双斜杠替换为单斜杠
    strutil::replace(fpath, "\\", "/");
    strutil::replace(fpath, "//", "/");

    return fpath; // 返回最终的资源文件路径
}
