#ifndef THREAD_H_
#define THREAD_H_
#include "ThreadBase.h"

namespace thread
{

class Thread : public ThreadBase
{
public:

	static void Sleep(unsigned long milliseconds);

	Thread(const char* name = nullptr, ThreadProcType proc = BaseProc) : ThreadBase(name, proc) { }
	virtual ~Thread() {}

	struct ThreadEventArgs
	{
		ThreadEventArgs() : m_Thread(nullptr), m_DataPtr(nullptr) { }
		Thread* m_Thread;
		void*   m_DataPtr;
	};

	typedef void(*ThreadEventFn)(ThreadEventArgs* arg);

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
	virtual void OnStart();
	virtual void Process();
	virtual void OnStop();

	struct ThreadEvent
	{
		ThreadEvent() : m_Fn(nullptr) { }
		ThreadEventArgs m_Args;
		ThreadEventFn   m_Fn;
	};

	ThreadEvent m_OnStart;
	ThreadEvent m_OnStop;
};

class ThreadCancelable : public Thread
{
public:
	ThreadCancelable(const char* name = nullptr) : Thread(name, CancelableProc) { }

protected:
	static ThreadProcReturnType THREADAPI CancelableProc(ThreadProcParamType ptr);
	static void Cleanup(void* ptr);
};

class ThreadEventListener
{
public:

	void Listen(Thread* thread)
	{
		thread->SetOnStart(ThreadOnStart, this);
		thread->SetOnStop(ThreadOnStop, this);
	}

	virtual void OnStartEvent(Thread* thread) = 0;
	virtual void OnStopEvent(Thread* thread) = 0;

	static void ThreadOnStart(Thread::ThreadEventArgs* arg)
	{
		if (arg != nullptr && arg->m_DataPtr != nullptr)
		{
			ThreadEventListener* self = static_cast<ThreadEventListener*>(arg->m_DataPtr);
			self->OnStartEvent(arg->m_Thread);
		}
	}

	static void ThreadOnStop(Thread::ThreadEventArgs* arg)
	{
		if (arg != nullptr && arg->m_DataPtr != nullptr)
		{
			ThreadEventListener* self = static_cast<ThreadEventListener*>(arg->m_DataPtr);
			self->OnStopEvent(arg->m_Thread);
		}
	}
};

} // namespace thread

#endif // THREAD_H_
