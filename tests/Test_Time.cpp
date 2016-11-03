#include <windows.h>
#include <stdio.h>
#include "../common/Log_Assert.h"
#include <chrono>

#pragma intrinsic(__rdtsc)
struct RDTSC_clock
{	
	typedef DWORD64 rep;
	typedef std::nano period;
	typedef std::ratio<1, 1> tick;
	typedef std::chrono::duration<rep, tick> nanoseconds;

	typedef std::chrono::nanoseconds duration;
	typedef std::chrono::time_point<RDTSC_clock> time_point;
	static constexpr bool is_steady = false;

	static time_point now() _NOEXCEPT
	{	
		const long long _Ctr = __rdtsc();;
		static_assert(period::num == 1, "This assumes period::num == 1.");
		return (time_point(duration(_Ctr)));
	}
};

struct ThreadCycle_clock
{
	typedef ULONG64 rep;
	typedef std::nano period;
	typedef std::ratio<1, 1> tick;
	typedef std::chrono::duration<rep, tick> nanoseconds;

	typedef std::chrono::nanoseconds duration;
	typedef std::chrono::time_point<RDTSC_clock> time_point;
	static constexpr bool is_steady = true;

	static time_point now() _NOEXCEPT
	{
		ULONG64 _Ctr;
		QueryThreadCycleTime(GetCurrentThread(), &_Ctr);
		static_assert(period::num == 1, "This assumes period::num == 1.");
		return (time_point(duration(_Ctr)));
	}
};

template <class tclock>
class TimerScope
{
public:
	TimerScope(typename tclock::duration& duration) : m_duration(duration) { m_time1 = tclock::now(); /*m_duration = m_duration.zero();*/ }
	~TimerScope() { m_time2 = tclock::now(); m_duration = m_time2 - m_time1; }

	typename tclock::time_point m_time1;
	typename tclock::time_point m_time2;
	typename tclock::duration& m_duration;
};

template <class tclock>
struct TimerOverhead
{
	DWORD64 get()
	{
		typename tclock::duration d;
		{
			TimerScope<tclock> ts(d);
		}
		return (DWORD64)d.count();
	}
};

int main_(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	{
		TimerOverhead<std::chrono::high_resolution_clock> d;
		printf("hres %lld\n", d.get());
	}

	{
		TimerOverhead<std::chrono::steady_clock> d;
		printf("steady %lld\n", d.get());
	}

	{
		TimerOverhead<RDTSC_clock> d;
		printf("%lld ticks\n", d.get());
	}

	{
		TimerOverhead<ThreadCycle_clock> d;
		printf("%lld ticks\n", d.get());
	}

	printf("Done\n");
	return 0;
}


