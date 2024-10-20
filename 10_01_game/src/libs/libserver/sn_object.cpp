#include "sn_object.h" // 引入 SnObject 类的定义
#include "global.h"    // 引入 Global 类，用于生成序列号

// SnObject 构造函数
SnObject::SnObject() {
    // 使用 Global 单例生成一个唯一的序列号，并赋值给成员变量 _sn
    _sn = Global::GetInstance()->GenerateSN();
}

// 获取当前对象的序列号
uint64 SnObject::GetSN() const {
    return _sn; // 返回成员变量 _sn 的值
}

// 重置序列号
void SnObject::ResetSN(bool isClean) {
    if (!isClean)
        // 如果 isClean 为 false，使用 Global 单例生成新的序列号
        _sn = Global::GetInstance()->GenerateSN();
    else
        // 如果 isClean 为 true，将序列号重置为 0
        _sn = 0;
}
