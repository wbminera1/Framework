#include <windows.h>
#include <stdio.h>
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"
#include "ThreadPool.h"
#include "../common/Log.h"

#include "Catch/single_include/catch.hpp"


TEST_CASE("ThreadTest", "[thread]")
{
	class TestThreadBase : public thread::ThreadBase
	{
	public:
		TestThreadBase() : ThreadBase(__FUNCTION__)
		{
			m_OnStartPassed = 
			m_OnStopPassed = 
			m_OnProcessStartedPassed = 
			m_OnProcessStoppedPassed = false;
		}

		virtual void OnStart()
		{
			m_OnStartPassed = true;
		}

		virtual void Process()
		{
			m_OnProcessStartedPassed = true;
			while (!m_Stop)
			{
				thread::Thread::Sleep(100);
			}
			m_OnProcessStoppedPassed = true;
		}

		virtual void OnStop()
		{
			m_OnStopPassed = true;
		}

		volatile bool m_OnStartPassed;
		volatile bool m_OnStopPassed;
		volatile bool m_OnProcessStartedPassed;
		volatile bool m_OnProcessStoppedPassed;
	};

	SECTION("ThreadBase")
	{
		TestThreadBase th1;
		th1.SetName("TestThreadBase");
		REQUIRE((th1.m_OnStartPassed == false && th1.m_OnStopPassed == false && th1.m_OnProcessStartedPassed == false && th1.m_OnProcessStoppedPassed == false) == true);
		REQUIRE(th1.Create() == 0);
		thread::Thread::Sleep(200);
		REQUIRE(th1.GetName() == "TestThreadBase");
		REQUIRE((th1.m_OnStartPassed == true && th1.m_OnStopPassed == false && th1.m_OnProcessStartedPassed == true && th1.m_OnProcessStoppedPassed == false) == true);
		th1.Stop();
		REQUIRE(th1.Join() == 0);
		REQUIRE((th1.m_OnStartPassed == true && th1.m_OnStopPassed == true && th1.m_OnProcessStartedPassed == true && th1.m_OnProcessStoppedPassed == true) == true);
	}

	class TestThread : public thread::Thread, public thread::ThreadEventListener
	{
	public:
		TestThread() : Thread(__FUNCTION__)
		{
			m_OnStartPassed =
			m_OnStopPassed =
			m_OnProcessStartedPassed =
			m_OnProcessStoppedPassed =
			m_OnStartEventPassed =
			m_OnStopEventPassed = false;

			Listen(this);
		}

		virtual void OnStart()
		{
			Thread::OnStart();
			m_OnStartPassed = true;
		}

		virtual void Process()
		{
			m_OnProcessStartedPassed = true;
			while (!m_Stop)
			{
				thread::Thread::Sleep(100);
			}
			m_OnProcessStoppedPassed = true;
		}

		virtual void OnStop()
		{
			Thread::OnStop();
			m_OnStopPassed = true;
		}

		void OnStartEvent(Thread* thread)
		{
			if (thread == this)
			{
				m_OnStartEventPassed = true;
			}
		}

		void OnStopEvent(Thread* thread)
		{
			if (thread == this)
			{
				m_OnStopEventPassed = true;
			}
		}

		volatile bool m_OnStartPassed;
		volatile bool m_OnStopPassed;
		volatile bool m_OnProcessStartedPassed;
		volatile bool m_OnProcessStoppedPassed;
		volatile bool m_OnStartEventPassed;
		volatile bool m_OnStopEventPassed;
	};

	SECTION("Thread")
	{
		TestThread th1;
		th1.SetName("TestThread");
		REQUIRE(th1.m_OnStartPassed == false);
		REQUIRE(th1.m_OnStopPassed == false);
		REQUIRE(th1.m_OnProcessStartedPassed == false);
		REQUIRE(th1.m_OnProcessStoppedPassed == false);
		REQUIRE(th1.m_OnStartEventPassed == false);
		REQUIRE(th1.m_OnStopEventPassed == false);
		REQUIRE(th1.Create() == 0);
		thread::Thread::Sleep(200);
		REQUIRE(th1.GetName() == "TestThread");
		REQUIRE(th1.m_OnStartPassed == true);
		REQUIRE(th1.m_OnStopPassed == false);
		REQUIRE(th1.m_OnProcessStartedPassed == true);
		REQUIRE(th1.m_OnProcessStoppedPassed == false);
		REQUIRE(th1.m_OnStartEventPassed == true);
		REQUIRE(th1.m_OnStopEventPassed == false);
		th1.Stop();
		REQUIRE(th1.Join() == 0);
		REQUIRE(th1.m_OnStartPassed == true);
		REQUIRE(th1.m_OnStopPassed == true);
		REQUIRE(th1.m_OnProcessStartedPassed == true);
		REQUIRE(th1.m_OnProcessStoppedPassed == true);
		REQUIRE(th1.m_OnStartEventPassed == true);
		REQUIRE(th1.m_OnStopEventPassed == true);
	}
}

void TestMutex()
{
	class TThread : public thread::Thread
	{
	public:
		TThread(const char* name, thread::Mutex& mutex, thread::Condition& cond)
			: Thread(__FUNCTION__)
			, m_Name(name)
			, m_Mutex(mutex)
			, m_Cond(cond)
		{

		}

		void Process()
		{
			Log(LOG_INFO, "Thread %s started", m_Name);
			m_Cond.Lock();
			Log(LOG_INFO, "Thread %s before condition", m_Name);
			m_Cond.TimedWait(3);
			Log(LOG_INFO, "Thread %s after condition", m_Name);
			m_Cond.Unlock();
			Log(LOG_INFO, "Thread %s stopped", m_Name);
		}

	private:
		const char* m_Name;
		thread::Mutex& m_Mutex;
		thread::Condition& m_Cond;
	};

	Log(LOG_INFO, "Test started");
	thread::Mutex mutex;
	thread::Condition cond;
	TThread t1("T1", mutex, cond);
	t1.Create();
	Sleep(2000);
	cond.Lock();
	Log(LOG_INFO, "Signaled");
	cond.Signal();
	cond.Unlock();
	t1.Join();
	Log(LOG_INFO, "Test stopped");
}

void TestPool()
{
	class TestThread : public thread::Thread
	{
	public:
		TestThread() : Thread(__FUNCTION__)
		{

		}

		void Process()
		{
			while (!m_Stop)
			{
				Log(LOG_INFO, "TestThread %lx", m_Thread);
				Sleep(100);
			}
		}

	};

	class TestThreadPool : public thread::ThreadPool<TestThread>
	{
		public:
			TestThreadPool(size_t size) : ThreadPool(size) {}
	};

	{
		TestThreadPool tp(10);
		Log(LOG_INFO, "ThreadPool creating");
		while (true)
		{
			TestThread* thr = tp.Create();
			if (thr == nullptr)
			{
				break;
			}
		}
		Log(LOG_INFO, "ThreadPool sleeping");
		Sleep(1000);
	}
	Log(LOG_INFO, "ThreadPool finished");
}
