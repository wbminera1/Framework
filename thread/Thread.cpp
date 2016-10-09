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

void Thread::Process()
{
	while (!m_Stop)
	{
		Sleep(1000);
	}
}

void* Thread::Start(void* ptr)
{
	Thread* self = static_cast<Thread*>(ptr);
	if (self->m_Name.size() > 0)
	{
		SetThreadName(-1, self->m_Name.c_str());
	}
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

void Thread::Cleanup(void* ptr)
{
	Thread* self = static_cast<Thread*>(ptr);
	if (self->m_OnStop.m_Fn != nullptr) {
		(*self->m_OnStop.m_Fn)(&self->m_OnStop.m_Args);
	}
}

} // namespace thread
