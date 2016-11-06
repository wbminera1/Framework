#include "Condition.h"

namespace thread
{

	Condition::Condition()
		: m_Condition(PTHREAD_COND_INITIALIZER)
	{
	}

	Condition::~Condition()
	{
		pthread_cond_destroy(&m_Condition);
	}

	int Condition::Wait()
	{
		return pthread_cond_wait(&m_Condition, &Get());
	}

	int Condition::TimedWait(time_t sec = 0, long nsec = 0)
	{
		timespec now, timeout;
		gettimeofday(&now);
		timeout.tv_sec = now.tv_sec + sec;
		timeout.tv_nsec = now.tv_nsec * nsec;
		return pthread_cond_timedwait(&m_Condition, &Get(), &timeout);
	}

	int Condition::Signal()
	{
		return pthread_cond_signal(&m_Condition);
	}

	int Condition::Broadcast()
	{
		return pthread_cond_broadcast(&m_Condition);
	}


} // namespace thread


