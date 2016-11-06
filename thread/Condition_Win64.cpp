#include "Condition.h"

namespace thread
{

	Condition::Condition()
	{
		InitializeConditionVariable(&m_Condition);
	}

	Condition::~Condition()
	{
	}

	int Condition::Wait()
	{
		BOOL res = SleepConditionVariableCS(&m_Condition, &Get(), INFINITE);
		return res != 0;
	}

	int Condition::TimedWait(time_t sec, long nsec)
	{
		BOOL res = SleepConditionVariableCS(&m_Condition, &Get(), (DWORD)sec * 1000 + nsec / 1000000);
		return res != 0; 
	}

	int Condition::Signal()
	{
		WakeConditionVariable(&m_Condition);
		return 1;
	}

	int Condition::Broadcast()
	{
		WakeAllConditionVariable(&m_Condition);
		return 1;
	}


} // namespace thread


