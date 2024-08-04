#include "object_block.h"

ObjectBlock::ObjectBlock(IDynamicObjectPool *pPool) {
    _pPool = pPool;
}
ObjectBlock::~ObjectBlock() {
    _pPool = nullptr;
}
void ObjectBlock::Dispose() {
    BackToPool();
}
