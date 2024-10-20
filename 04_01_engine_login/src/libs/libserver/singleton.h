#pragma once

#include <stdexcept>
#include <utility>

template <typename T> class Singleton {
public:
  // 获取单例实例。如果实例不存在，则创建一个新的实例
  template <typename... Args> static T *Instance(Args &&...args) {
    if (m_pInstance == nullptr)
      m_pInstance = new T(std::forward<Args>(args)...);
    return m_pInstance;
  }

  // 获取单例实例。如果实例不存在，则抛出异常
  static T *GetInstance() {
    if (m_pInstance == nullptr)
      throw std::logic_error("the instance is not initialized, please "
                             "initialize the instance first");
    return m_pInstance;
  }

  // 销毁单例实例
  static void DestroyInstance() {
    delete m_pInstance;
    m_pInstance = nullptr;
  }

private:
  // 单例实例的指针，初始化为nullptr
  static T *m_pInstance;
};

// 静态成员变量的定义，初始化为nullptr
template <class T> T *Singleton<T>::m_pInstance = nullptr;

/**
TODO

线程安全: 这个实现不是线程安全的。如果在多线程环境中使用
Singleton，需要额外的同步机制来确保线程安全，例如使用互斥锁（std::mutex）来保护对
m_pInstance 的访问。 内存管理: DestroyInstance
方法负责释放单例实例的内存。如果程序需要频繁创建和销毁单例实例，可能需要考虑更复杂的内存管理策略。
防止拷贝:
该实现不允许拷贝或赋值，因为没有提供拷贝构造函数或赋值操作符。如果需要防止拷贝，应该禁用它们。
 */