#include <windows.h>
#include <stdio.h>
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include "ThreadPool.h"
#include "../common/Log.h"


void TestThread()
{
	class TestThread : public thread::Thread
	{
		void Process()
		{
			Log(LOG_INFO, "Sizeof %d\n", sizeof(this->m_Thread));
			Log(LOG_INFO, "Thread %lx\n", m_Thread);
		}
	};
	TestThread th1, th2;
	th1.Create();
	th2.Create();
	Sleep(100);
	th1.Join();
	th2.Join();
}

void TestMutex()
{
	class TThread : public thread::Thread
	{
	public:
		TThread(const char* name, thread::Mutex& mutex, thread::Condition& cond)
			: m_Name(name)
			, m_Mutex(mutex)
			, m_Cond(cond)
		{

		}

		void Process()
		{
			Log(LOG_INFO, "Thread %s started\n", m_Name);
			m_Cond.Lock();
			Log(LOG_INFO, "Thread %s before condition\n", m_Name);
			m_Cond.TimedWait(3);
			Log(LOG_INFO, "Thread %s after condition\n", m_Name);
			m_Cond.Unlock();
			Log(LOG_INFO, "Thread %s stopped\n", m_Name);
		}

	private:
		const char* m_Name;
		thread::Mutex& m_Mutex;
		thread::Condition& m_Cond;
	};

	Log(LOG_INFO, "Test started\n");
	thread::Mutex mutex;
	thread::Condition cond;
	TThread t1("T1", mutex, cond);
	t1.Create();
	Sleep(2000);
	cond.Lock();
	Log(LOG_INFO, "Signaled\n");
	cond.Signal();
	cond.Unlock();
	t1.Join();
	Log(LOG_INFO, "Test stopped\n");
}

void TestThreadPool()
{
	class TestThread : public thread::Thread
	{
		void Process()
		{
			while (!m_Stop)
			{
				Log(LOG_INFO, "TestThread %lx\n", m_Thread);
				Sleep(100);
			}
		}
	};

	thread::ThreadPool<TestThread> tp(10);
	Log(LOG_INFO, "ThreadPool creating\n");
	tp.Create();
	Log(LOG_INFO, "ThreadPool sleeping\n");
	Sleep(1000);
	Log(LOG_INFO, "ThreadPool stopping\n");
	tp.Stop();
	Log(LOG_INFO, "ThreadPool joining\n");
	tp.Join();
	Log(LOG_INFO, "ThreadPool finished\n");
}


int main(int argc, char** argv)
{
	//TestThread();
	//TestMutex();
	TestThreadPool();
	return 0;
}
