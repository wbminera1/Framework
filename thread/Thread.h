#ifndef THREAD_H_
#define THREAD_H_
#include <string.h>
#include <excpt.h>
#include <pthread.h>

namespace thread
{

class Thread
{
    public:
		struct ThreadEventArgs
		{
			ThreadEventArgs() : m_Thread(nullptr), m_DataPtr(nullptr) { }
			Thread* m_Thread;
			void*   m_DataPtr;
		};

		typedef void (*ThreadEventFn)(ThreadEventArgs* arg);

		struct ThreadEvent
		{
			ThreadEvent() : m_Fn(nullptr) { }
			ThreadEventArgs m_Args;
			ThreadEventFn   m_Fn;
		};

        Thread() 
			: m_Stop(false)
		{ 
			memset(&m_Thread, 0, sizeof(m_Thread));
		}

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

		void SetOnStart(ThreadEventFn onStart, void* dataPtr = nullptr)
		{
			m_OnStart.m_Fn = onStart;
			m_OnStart.m_Args.m_Thread = this;
			m_OnStart.m_Args.m_DataPtr = dataPtr;
		}

		void SetOnStop(ThreadEventFn onStop, void* dataPtr = nullptr)
		{
			m_OnStop.m_Fn = onStop;
			m_OnStop.m_Args.m_Thread = this;
			m_OnStop.m_Args.m_DataPtr = dataPtr;
		}

    protected:

        virtual void Process() = 0;

        volatile bool m_Stop;

		ThreadEvent m_OnStart;
		ThreadEvent m_OnStop;
        pthread_t m_Thread;

        static void* Start ( void* ptr )
        {
			Thread* self = static_cast<Thread*>(ptr);
			pthread_cleanup_push(Cleanup, ptr);
			self->m_Stop = false;
			if (self->m_OnStart.m_Fn != nullptr) {
				(*self->m_OnStart.m_Fn)(&self->m_OnStart.m_Args);
			}
            self->Process();
			self->m_Stop = true;
			pthread_cleanup_pop(1);
            return NULL;
        }

		static void Cleanup(void* ptr)
		{
			Thread* self = static_cast<Thread*>(ptr);
			if (self->m_OnStop.m_Fn != nullptr) {
				(*self->m_OnStop.m_Fn)(&self->m_OnStop.m_Args);
			}
		}

};

} // namespace thread

#endif // THREAD_H_
