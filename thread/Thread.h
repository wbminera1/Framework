#ifndef THREAD_H_
#define THREAD_H_
#include <pthread.h>

namespace thread
{

class Thread
{
    public:
        Thread() : m_Stop(false) { }
        virtual ~Thread() { }

        int Create()
        {
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            int res = pthread_create(&m_Thread, &attr, &Thread::Start, (void *) this);
            pthread_attr_destroy(&attr);
            return res;
        }

        int Join()
        {
            void* status;
            int res = pthread_join(m_Thread, &status);
            return res;
        }

        void Stop()
        {
            m_Stop = true;
        }

    protected:

        virtual void Process() = 0;

        volatile bool m_Stop;

        pthread_t m_Thread;

        static void* Start ( void *ptr )
        {
			Thread* self = static_cast<Thread*>(ptr);
			self->m_Stop = false;
            self->Process();
            return NULL;
        }
};

} // namespace thread

#endif // THREAD_H_
