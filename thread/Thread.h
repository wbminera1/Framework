#ifndef THREAD_H_
#define THREAD_H_
#include <string.h>
#include <string>
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

		typedef void(*ThreadEventFn)(ThreadEventArgs* arg);

        Thread(const char* name = nullptr) 
			: m_Stop(false)
		{ 
			memset(&m_Thread, 0, sizeof(m_Thread));
			SetName(name);
		}

        virtual ~Thread() { }

		void SetName(const char* name)
		{
			if (name != nullptr)
			{
				m_Name = name;
			}
		}

		virtual int Create()
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

		virtual void Stop()
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
		virtual void Process();

		struct ThreadEvent
		{
			ThreadEvent() : m_Fn(nullptr) { }
			ThreadEventArgs m_Args;
			ThreadEventFn   m_Fn;
		};

        volatile bool m_Stop;

		ThreadEvent m_OnStart;
		ThreadEvent m_OnStop;
        pthread_t m_Thread;
		std::string m_Name;

		static void* Start(void* ptr);
		static void Cleanup(void* ptr);

};

} // namespace thread

#endif // THREAD_H_
