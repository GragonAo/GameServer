#pragma once

#include "common.h" // 引入常用的头文件，可能包含常量和类型定义

// SnObject 类定义
class SnObject {
public:
    // 构造函数，初始化对象
    SnObject();

    // 获取当前对象的序列号
    uint64 GetSN() const;

    // 重置序列号，可以选择是否清空
    void ResetSN(bool isClean = false);

protected:
    // 序列号，通常用于唯一标识对象
    uint64 _sn;
};
