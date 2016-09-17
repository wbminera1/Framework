#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_
#include "Thread.h"
#include "Condition.h"

namespace thread
{

class Task
{
    public:
        virtual void Start() = 0;
};

class TaskQueue : public Thread
{
    public:

        virtual void DoTask() = 0;

        void StartTask()
        {
            m_WaitStart.Lock();
            m_WaitStart.Signal();
            m_WaitStart.Unlock();
        }

        virtual void Process()
        {
            m_WaitStart.Lock();
            while (!m_Stop)
            {
                m_WaitStart.Wait();
                DoTask();
            }
            m_WaitStart.Unlock();
        }
    private:
        Condition m_WaitStart;
};

} //namespace thread

#endif // TASKQUEUE_H_
