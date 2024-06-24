//确保头文件在编译期间只包含一次，防止重复定义。
#pragma once
//让任何继承 IDisposable 的类都必须实现 Dispose 方法，从而确保资源的正确释放
class IDisposable{
    public:
    virtual ~IDisposable() = default; // 虚拟析构函数，确保派生类正确析构
    virtual void Dispose() = 0;       // 纯虚函数，要求派生类实现
};