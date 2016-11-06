#ifndef BARRIER_H_
#define BARRIER_H_
#ifdef _WIN64
#include <windows.h>
#ifdef WINDOWS8
typedef SYNCHRONIZATION_BARRIER	BarrierType;
#else
struct BarrierType
{
	CRITICAL_SECTION m_CriticalSection;
	CONDITION_VARIABLE m_Condition;
	volatile unsigned int m_Count;
};
#endif WINDOWS8
#endif
#ifdef __unix__
#include <pthread.h>
typedef pthread_barrier_t BarrierType;
#endif

namespace thread
{
	class Barrier
	{
	public:
		Barrier(unsigned int count);
		~Barrier();

		void Wait();

	private:
		BarrierType m_Barrier;
	};

} // namespace thread

#endif // BARRIER_H_