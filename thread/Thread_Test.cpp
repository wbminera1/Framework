#include <windows.h>
#include <stdio.h>
#include "Thread.h"
#include "Mutex.h"
#include "CriticalSection.h"
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
		REQUIRE(th1.Create());
		thread::Thread::Sleep(200);
		REQUIRE(th1.GetName() == "TestThreadBase");
		REQUIRE((th1.m_OnStartPassed == true && th1.m_OnStopPassed == false && th1.m_OnProcessStartedPassed == true && th1.m_OnProcessStoppedPassed == false) == true);
		th1.Stop();
		REQUIRE(th1.Join());
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
		REQUIRE(th1.Create());
		thread::Thread::Sleep(200);
		REQUIRE(th1.GetName() == "TestThread");
		REQUIRE(th1.m_OnStartPassed == true);
		REQUIRE(th1.m_OnStopPassed == false);
		REQUIRE(th1.m_OnProcessStartedPassed == true);
		REQUIRE(th1.m_OnProcessStoppedPassed == false);
		REQUIRE(th1.m_OnStartEventPassed == true);
		REQUIRE(th1.m_OnStopEventPassed == false);
		th1.Stop();
		REQUIRE(th1.Join());
		REQUIRE(th1.m_OnStartPassed == true);
		REQUIRE(th1.m_OnStopPassed == true);
		REQUIRE(th1.m_OnProcessStartedPassed == true);
		REQUIRE(th1.m_OnProcessStoppedPassed == true);
		REQUIRE(th1.m_OnStartEventPassed == true);
		REQUIRE(th1.m_OnStopEventPassed == true);
	}

	class TestThreadPool : public thread::ThreadPool<TestThread>
	{
	public:
		TestThreadPool(size_t size) : ThreadPool(size) {}
	};

	SECTION("ThreadPool")
	{
		const size_t ttpSize = 4;
		TestThreadPool ttp(ttpSize);

		for (size_t i = 0; i < ttpSize; ++i)
		{
			REQUIRE(ttp.Create() != nullptr);
		}
		REQUIRE(ttp.Create() == nullptr);
		Sleep(200); //!!Barrier
		ttp.StopAll();
		ttp.WaitAll();
		ttp.JoinAll();

		for (size_t i = 0; i < ttpSize; ++i)
		{
			REQUIRE(ttp.Create() != nullptr);
		}
		REQUIRE(ttp.Create() == nullptr);
	}

}
template <class sync_type>
class SyncTestThread : public thread::ThreadBase
{
public:
	SyncTestThread(sync_type& syncVar, unsigned int& counter)
		: ThreadBase(__FUNCTION__)
		, m_SyncVar(syncVar)
		, m_Counter(counter)
	{   }
private:
	void Process()
	{
		thread::Thread::Sleep(0);
		for (size_t i = 0; i < 1024; ++i)
		{
			m_SyncVar.Lock();
			++m_Counter;
			m_SyncVar.Unlock();
			thread::Thread::Sleep(0);
		}
		for (size_t i = 0; i < 1024; ++i)
		{
			while (!m_SyncVar.TryLock())
			{
				thread::Thread::Sleep(0);
			}
			++m_Counter;
			m_SyncVar.Unlock();
		}
	}
	sync_type& m_SyncVar;
	unsigned int& m_Counter;
};

template <class sync_type>
bool SyncTestFunc()
{
	static sync_type sync;
	static unsigned int syncCount = 0;
	SyncTestThread<sync_type>* threads[32];
	for (size_t i = 0; i < 32; ++i) {
		threads[i] = new SyncTestThread<sync_type>(sync, syncCount);
	}
	for (size_t i = 0; i < 32; ++i) {
		threads[i]->Create();
	}
	for (size_t i = 0; i < 32; ++i) {
		threads[i]->Join();
	}
	for (size_t i = 0; i < 32; ++i) {
		delete threads[i];
	}
	return syncCount == 2 * 32 * 1024;
}

TEST_CASE("SyncTest", "[thread]")
{
	class NoLock
	{
	public:
		bool TryLock() { return true; }
		void Lock() { }
		void Unlock() { }
	};

	SECTION("NoLock")
	{
		REQUIRE(SyncTestFunc<NoLock>() == false);
	}

	SECTION("Mutex")
	{
		REQUIRE(SyncTestFunc<thread::Mutex>() == true);
	}

	SECTION("CriticalSection")
	{
		REQUIRE(SyncTestFunc<thread::CriticalSection>() == true);
	}

}
