#pragma once
#include <queue>
#include <iostream>
#include <sstream>
#include <list>

#include "sn_object.h"
#include "object_block.h"
#include "packet.h"
#include "object_pool_interface.h"
#include "thread_obj.h"
#include "cache_refresh.h"
#include "object_pool_mgr.h"

template <typename T>
class DynamicObjectPool :public IDynamicObjectPool
{
public:
    static DynamicObjectPool<T>* GetInstance()
    {
        std::lock_guard<std::mutex> guard(_instanceLock);

        if (_pInstance == nullptr)
        {
            _pInstance = new DynamicObjectPool<T>();
            DynamicObjectPoolMgr::GetInstance()->AddPool(_pInstance);
        }

        return _pInstance;
    }

    void DestroyInstance() override
    {
        std::lock_guard<std::mutex> guard(_instanceLock);
        if (_pInstance == nullptr)
            return;

        delete _pInstance;
        _pInstance = nullptr;
    }

    DynamicObjectPool();
    ~DynamicObjectPool();

    template<typename ...Targs>
    T* MallocObject(Targs... args);

    void Update() override;
    void FreeObject(ObjectBlock* pObj) override;

    void Show();

private:
    void CreateOne();

private:
    std::queue<T*> _free;
    std::mutex _freeLock;

    std::mutex _inUseLock;
    CacheRefresh<T> _objInUse;

    static DynamicObjectPool<T>* _pInstance;
    static std::mutex _instanceLock;
};

template <typename T>
DynamicObjectPool<T>* DynamicObjectPool<T>::_pInstance = nullptr;

template <typename T>
std::mutex DynamicObjectPool<T>::_instanceLock;


template <typename T>
DynamicObjectPool<T>::DynamicObjectPool()
{
}

template <typename T>
void DynamicObjectPool<T>::CreateOne()
{
    T* pObj = new T(this);
    if (!pObj) {
        std::cerr << "Failed to create new object." << std::endl;
        return;
    }
    // std::cout << "Created new object at address: " << pObj << std::endl;
    _free.push(pObj);
}

template <typename T>
DynamicObjectPool<T>::~DynamicObjectPool()
{
    Update();

    // std::cout << "dispose object pool. type:" << typeid(T).name() << std::endl;

    while (_free.size() > 0)
    {
        auto iter = _free.front();
        delete iter;
        _free.pop();
    }
    _objInUse.Dispose();
}

template <typename T>
template <typename ... Targs>
T* DynamicObjectPool<T>::MallocObject(Targs... args)
{
    // std::cout << "MallocObject called with args: ..." << std::endl;

    _freeLock.lock();
    if (_free.size() == 0)
    {
        // std::cout << "Creating one object since pool is empty." << std::endl;
        CreateOne();
    }

    auto pObj = _free.front();
    _free.pop();
    _freeLock.unlock();

    if (!pObj) {
        std::cerr << "Failed to allocate object from pool." << std::endl;
        return nullptr;
    }

    // std::cout << "Allocated object at address: " << pObj << std::endl;

    pObj->ResetSN();
    pObj->TakeoutFromPool(std::forward<Targs>(args)...);

    _inUseLock.lock();
    _objInUse.GetAddCache()->push_back(pObj);
    _inUseLock.unlock();

    return pObj;
}


template <typename T>
void DynamicObjectPool<T>::Update()
{
    std::list<T*> freeObjs;
    _inUseLock.lock();
    if (_objInUse.CanSwap())
    {
        freeObjs = _objInUse.Swap();
    }
    _inUseLock.unlock();

    std::lock_guard<std::mutex> guard(_freeLock);
    for (auto one : freeObjs)
    {
        if(one)
            _free.push(one);
        else
            std::cout<<"回收对象为空,不能加入空闲队列"<<std::endl;
    }
}

template<typename T>
inline void DynamicObjectPool<T>::FreeObject(ObjectBlock* pObj)
{
    std::lock_guard<std::mutex> guard(_inUseLock);
    T* castedObj = dynamic_cast<T*>(pObj);
    if (castedObj) {
        _objInUse.GetRemoveCache()->emplace_back(castedObj);
    } else {
        std::cerr << "Failed to cast ObjectBlock to the target type" << std::endl;
        // Handle error, possibly throw an exception or log an error
    }
}

template <typename T>
void DynamicObjectPool<T>::Show()
{
    std::lock_guard<std::mutex> guard(_freeLock);
    std::lock_guard<std::mutex> guardInUse(_inUseLock);
    auto count = _objInUse.GetReaderCache()->size() + _objInUse.GetAddCache()->size() + _objInUse.GetRemoveCache()->size();

    std::stringstream log;
    log << "*************************** " << "\n";
    log << "pool total count:\t" << _free.size() + count << "\n";
    log << "free count:\t\t" << _free.size() << "\n";
    log << "in use count:\t" << count << "\n";

    std::cout << log.str() << std::endl;
}

