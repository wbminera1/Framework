#include <windows.h>
#include <excpt.h>

#include "ThreadBase.h"

namespace {

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

}


namespace thread
{
	void ThreadBase::OnStart()
	{
		m_Stop = false;
		if (m_Name.size() > 0)
		{
			SetThreadName(-1, m_Name.c_str());
		}
	}

	void* ThreadBase::Start(void* ptr)
	{
		ThreadBase* self = static_cast<ThreadBase*>(ptr);
		self->OnStart();
		self->Process();
		self->OnStop();
		return NULL;
	}

	void ThreadBase::OnStop()
	{

	}


} // namespace thread