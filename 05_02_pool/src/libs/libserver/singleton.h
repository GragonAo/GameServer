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
