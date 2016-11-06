#include "Mutex.h"

namespace thread
{

	Mutex::Mutex()
	{
		m_Mutex = CreateMutex( NULL, FALSE, NULL);
	}

	Mutex::~Mutex()
	{
		CloseHandle(m_Mutex);
	}


	bool Mutex::TryLock()
	{
		DWORD res = WaitForSingleObject(m_Mutex, 0);
		return (res == WAIT_OBJECT_0);
	}

	void Mutex::Lock()
	{
		/*DWORD res = */
		WaitForSingleObject(m_Mutex, INFINITE);
	}

	void Mutex::Unlock()
	{
		ReleaseMutex(m_Mutex);
	}

} // namespace thread
