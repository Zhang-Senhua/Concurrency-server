#include "Thread.h"
#include "AutoLock.h"
using namespace yazi::thread;
#include "Logger.h"
using namespace yazi::utility;
Thread::Thread() : m_tid(0), m_task(NULL)
{
}

Thread::~Thread()
{
}

void Thread::start()
{

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ERROR = pthread_create(&m_tid, &attr, thread_func, (void *)this);
    if (ERROR)
    {
        debug("creat error!ERROR NUM=%d", ERROR);
    }
    pthread_attr_destroy(&attr);
}

void Thread::stop()
{
    pthread_exit(PTHREAD_CANCELED);
}

void *Thread::thread_func(void *ptr)
{
    // static int count = 0;
    Thread *thread = (Thread *)ptr;
    // debug("调用子类taskdispatcher的run函数！");
    // count++;
    // debug("线程数量%d", count);
    thread->run();
    return ptr;
}

void Thread::set_task(Task *task)
{
    m_mutex.lock();
    m_task = task;
    m_cond.signal();
    m_mutex.unlock();
}

Task *Thread::get_task()
{
    AutoLock lock(&m_mutex);
    return m_task;
}
