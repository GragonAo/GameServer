#include "thread_obj.h"
#include "message_list.h"

bool ThreadObject::IsActive() const { return _active; }

void ThreadObject::Dispose() {
    _active = false;
    MessageList::Dispose();
}

void ThreadObject::SetThread(Thread *pThread) { _pThread = pThread; }

Thread *ThreadObject::GetThread() const { return _pThread; }