#pragma once

#include "disposable.h"
#include "sn_object.h"

class IDynamicObjectPool;

class ObjectBlock : virtual public SnObject, virtual public IDisposable {
public:
  ObjectBlock(IDynamicObjectPool *pPool);
  virtual ~ObjectBlock();
  virtual void Dispose() override;
  virtual void BackToPool() = 0;

protected:
  IDynamicObjectPool *_pPool{nullptr};
};