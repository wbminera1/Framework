#ifndef CONDITION_H_
#define CONDITION_H_
#include "CriticalSection.h"
#include "Misc.h"
#ifdef _WIN64
#include <windows.h>
typedef CONDITION_VARIABLE ConditionType;
#endif
#ifdef __unix__
#include <pthread.h>
typedef pthread_cond_t ConditionType;
#endif

namespace thread
{
	
class Condition : public CriticalSection
{
	public:
		Condition();
		~Condition();

		int Wait();
		int TimedWait(time_t sec = 0, long nsec = 0);

		int Signal();
		int Broadcast();
	
	private:
		ConditionType m_Condition;
};

} // namespace thread

#endif // CONDITION_H_
