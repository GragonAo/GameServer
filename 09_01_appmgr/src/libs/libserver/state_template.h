#pragma once

#include <map>

// 宏定义，用于动态创建状态对象并绑定状态类型
#define DynamicStateCreate(classname, enumType)                                \
  static void *CreateState() { return new classname; }                         \
  RobotStateType GetState() override { return enumType; }

#define DynamicStateBind(classname)                                            \
  reinterpret_cast<CreateIstancePt>(&(classname::CreateState))

// 模板类 StateTemplate，定义了状态的基础接口
template <typename enumType, class T> class StateTemplate {
public:
  StateTemplate() {_pParentObj = nullptr;}
  void SetParentObj(T *pObj) { _pParentObj = pObj; } // 设置父对象
  virtual ~StateTemplate() {}
  virtual enumType GetState() = 0; // 获取当前状态类型
  virtual enumType Update() = 0; // 更新状态，返回新的状态类型
  virtual void EnterState() = 0; // 进入状态
  virtual void LeaveState() = 0; // 离开状态

protected:
  T *_pParentObj{nullptr}; // 父对象指针
};

// 状态管理器模板类，管理状态的切换和更新
template <typename enumType, class StateClass, class T> class StateTemplateMgr {
public:
  virtual ~StateTemplateMgr() {
    if (_pState != nullptr) {
      delete _pState; // 删除当前状态对象
    }
  }
  void InitStateTemplateMgr(enumType defaultState) {
    _defaultState = defaultState;
    RegisterState(); // 注册所有状态
  }
  void ChangeState(enumType stateType) {
    StateClass *pNewState = CreateStateObj(stateType);
    if (pNewState == nullptr) {
      return;
    }
    if (pNewState != nullptr) {
      if (_pState != nullptr) {
        _pState->LeaveState(); // 离开当前状态
        delete _pState;
      }
      _pState = pNewState; // 切换到新状态
      _pState->EnterState(); // 进入新状态
    }
  }
  void UpdateState() {
    if (_pState == nullptr) {
      ChangeState(_defaultState); // 切换到默认状态
    }
    enumType curState = _pState->Update(); // 更新当前状态
    if (curState != _pState->GetState()) {
      ChangeState(curState); // 如果状态改变，切换到新状态
    }
  }

protected:
  virtual void RegisterState() = 0; // 纯虚函数，注册所有状态


//////////////////////////////////////////////////////////////////////////////////////////

public:
  typedef StateClass *(*CreateIstancePt)(); // 函数指针类型，用于创建状态对象
  StateClass *CreateStateObj(enumType enumValue) {
    auto iter = _dynCreateMap.find(enumValue);
    if (iter == _dynCreateMap.end())
      return nullptr;
    CreateIstancePt np = iter->second;
    StateClass *pState = np();
    pState->SetParentObj(static_cast<T *>(this)); // 设置父对象
    return pState;
  }
  void RegisterStateClass(enumType enumValue, CreateIstancePt np) {
    _dynCreateMap[enumValue] = np; // 注册状态类和创建函数
  }

protected:
  std::map<enumType, CreateIstancePt> _dynCreateMap; // 动态创建函数映射
  StateClass *_pState{nullptr}; // 当前状态对象
  enumType _defaultState; // 默认状态类型
};
