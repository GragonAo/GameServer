#pragma once

#include "component.h"
#include "disposable.h"

class IDynamicObjectPool : public IDisposable {
public:
  virtual void Update() = 0;
  virtual void FreeObject(IComponent *pObj) = 0;
  virtual void Show() = 0;
};