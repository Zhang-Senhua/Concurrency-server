#include "TaskDispatcher.h"
#include "Logger.h"
#include "Singleton.h"

using namespace yazi::utility;
using namespace yazi::thread;

TaskDispatcher::TaskDispatcher()
{
}

TaskDispatcher::~TaskDispatcher()
{
}

void TaskDispatcher::init(int threads)
{
    Singleton<ThreadPool>::instance()->create(threads);
    start();
}

void TaskDispatcher::assign(Task *task)
{
    debug("task dispatcher assign task");
    m_mutex.lock();
    m_tasks.push_back(task);
    m_mutex.unlock();
    m_cond.signal(); //发出信号，发生变化，通知唤醒阻塞的地方
}

void TaskDispatcher::handle(Task *task)
{
    // debug("task dispatcher handle task");
    ThreadPool *threadpool = Singleton<ThreadPool>::instance();
    if (threadpool->get_idle_thread_numbers() > 0)
    {
        threadpool->assign(task);
    }
    else
    {
        m_mutex.lock();
        m_tasks.push_front(task); //繁忙将任务重新入队列
        m_mutex.unlock();
        // debug("all threads are busy!");
    }
}

void TaskDispatcher::run()
{
    debug("TaskDispatcher::run");
    sigset_t mask;
    if (0 != sigfillset(&mask))
    {
        error("thread manager sigfillset failed!");
        return;
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    {
        error("thread manager pthread_sigmask failed!");
        return;
    }
    while (true)
    {
        // debug("task list: %d", m_actions.size());
        m_mutex.lock();
        while (m_tasks.empty())
        {
            // m_tasks是父对象thread的变量
            m_cond.wait(&m_mutex); //使用条件变量进行阻塞，条件发生变化时被唤醒，不用一直加锁解锁
            debug("wait for m_task!");
        }
        Task *task = m_tasks.front();
        debug("task pop!");
        m_tasks.pop_front(); //从前面弹出元素（删除）
        m_mutex.unlock();
        handle(task);
    }
}
