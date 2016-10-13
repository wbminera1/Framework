#ifndef THREAD_H_
#define THREAD_H_
#include <string.h>
#include <string>
#include <pthread.h>

namespace thread
{

class ThreadBase
{
    public:

		ThreadBase(const char* name = nullptr)
			: m_Stop(false)
		{ 
			memset(&m_Thread, 0, sizeof(m_Thread));
			SetName(name);
		}

        virtual ~ThreadBase() { }

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
            int res = pthread_create(&m_Thread, &attr, &ThreadBase::Start, (void *) this);
            pthread_attr_destroy(&attr);
            return res;
        }

        virtual int Join()
        {
            void* status;
            int res = pthread_join(m_Thread, &status);
            return res;
        }

		virtual void Stop()
        {
            m_Stop = true;
        }

protected:
		virtual void OnCreate();
		virtual void Process() = 0;
		virtual void OnCleanup();

        volatile bool m_Stop;

        pthread_t m_Thread;
		std::string m_Name;

		static void* Start(void* ptr);

};

class Thread : public ThreadBase
{
public:

	struct ThreadEventArgs
	{
		ThreadEventArgs() : m_Thread(nullptr), m_DataPtr(nullptr) { }
		Thread* m_Thread;
		void*   m_DataPtr;
	};

	typedef void(*ThreadEventFn)(ThreadEventArgs* arg);

	Thread(const char* name = nullptr) : ThreadBase(name) { }
	virtual ~Thread() {}

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
	virtual void OnCreate();
	virtual void Process();
	virtual void OnCleanup();

	virtual void ProcessManaged();

	struct ThreadEvent
	{
		ThreadEvent() : m_Fn(nullptr) { }
		ThreadEventArgs m_Args;
		ThreadEventFn   m_Fn;
	};

	ThreadEvent m_OnStart;
	ThreadEvent m_OnStop;

	static void Cleanup(void* ptr);

};

class ThreadEventListener
{
public:
	virtual void OnStart(Thread* thread) = 0;
	virtual void OnStop(Thread* thread) = 0;

	static void ThreadOnStart(Thread::ThreadEventArgs* arg)
	{
		if (arg != nullptr && arg->m_DataPtr != nullptr)
		{
			ThreadEventListener* self = static_cast<ThreadEventListener*>(arg->m_DataPtr);
			self->OnStart(arg->m_Thread);
		}
	}

	static void ThreadOnStop(Thread::ThreadEventArgs* arg)
	{
		if (arg != nullptr && arg->m_DataPtr != nullptr)
		{
			ThreadEventListener* self = static_cast<ThreadEventListener*>(arg->m_DataPtr);
			self->OnStop(arg->m_Thread);
		}
	}
};

} // namespace thread

#endif // THREAD_H_
