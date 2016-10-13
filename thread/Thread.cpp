#include <windows.h>
#include <excpt.h>

#include "Thread.h"
namespace thread
{

	const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
#pragma pack(pop)
	void SetThreadName(DWORD dwThreadID, const char* threadName) {
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
		__try {
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
		}
#pragma warning(pop)
	}

	void ThreadBase::OnCreate()
	{
		if (m_Name.size() > 0)
		{
			SetThreadName(-1, m_Name.c_str());
		}
		Process();
	}

	void ThreadBase::OnCleanup()
	{

	}

	void* ThreadBase::Start(void* ptr)
	{
		ThreadBase* self = static_cast<ThreadBase*>(ptr);
		self->OnCreate();
		return NULL;
	}

void Thread::Process()
{
	m_Stop = false;
	pthread_cleanup_push(Cleanup, this);
	ProcessManaged();
	pthread_cleanup_pop(1);
}

void Thread::ProcessManaged()
{
	while (!m_Stop)
	{
		Sleep(1000);
	}
}

void Thread::OnCreate()
{
	ThreadBase::OnCreate();
	if (m_OnStart.m_Fn != nullptr) {
		(*m_OnStart.m_Fn)(&m_OnStart.m_Args);
	}
}

void Thread::OnCleanup()
{

}

void Thread::Cleanup(void* ptr)
{
	Thread* self = static_cast<Thread*>(ptr);
	if (self->m_OnStop.m_Fn != nullptr) {
		(*self->m_OnStop.m_Fn)(&self->m_OnStop.m_Args);
	}
}

} // namespace thread
