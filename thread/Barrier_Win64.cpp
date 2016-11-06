#include "../common/Log.h"

#include "Barrier.h"

namespace thread
{
#ifdef WINDOWS8
	Barrier::Barrier(unsigned int count)
	{
		InitializeSynchronizationBarrier(&m_Barrier, (LONG)count, 0x400);
	}

	Barrier::~Barrier()
	{
		DeleteSynchronizationBarrier(&m_Barrier);
	}

	void Barrier::Wait()
	{
		EnterSynchronizationBarrier(&m_Barrier, 0);
	}
#else
	Barrier::Barrier(unsigned int count)
	{
		m_Barrier.m_Count = count;
		InitializeConditionVariable(&m_Barrier.m_Condition);
		InitializeCriticalSectionAndSpinCount(&m_Barrier.m_CriticalSection, 0x100);
	}

	Barrier::~Barrier()
	{
		DeleteCriticalSection(&m_Barrier.m_CriticalSection);
	}

	void Barrier::Wait()
	{
		EnterCriticalSection(&m_Barrier.m_CriticalSection);
		if (m_Barrier.m_Count <= 1)
		{
			m_Barrier.m_Count = 0;
			WakeAllConditionVariable(&m_Barrier.m_Condition);
		}
		else
		{
			--m_Barrier.m_Count;
			SleepConditionVariableCS(&m_Barrier.m_Condition, &m_Barrier.m_CriticalSection, INFINITE);
		}
		LeaveCriticalSection(&m_Barrier.m_CriticalSection);
	}
#endif // WINDOWS8

} // namespace thread


