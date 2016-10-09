#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <vector>
#include "../common/Log_Assert.h"
#include "Thread.h"
#include "Mutex.h"

namespace thread
{

template <class T>
class ThreadPool
{
	public:

		ThreadPool(size_t size)
	    {
	        m_StoppedThreads.reserve(size);
			m_StartingThreads.reserve(size);
			m_StartedThreads.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				T* threadPtr = new T();
				threadPtr->SetOnStart(ThreadOnStart, this);
				threadPtr->SetOnStop(ThreadOnStop, this);
				m_StoppedThreads.push_back(threadPtr);
			}
		}
	
	    virtual ~ThreadPool()
	    {
			Log(LOG_INFO, __FUNCTION__);
			StopAll();
			WaitAll();
			JoinAll();
			{
				LockGuard<Mutex> mutexLock(m_ThreadsLock);
				Log_Assert(LOG_ERR, m_StoppedThreads.size() == m_StoppedThreads.capacity());
				for (size_t i = 0; i < m_StoppedThreads.size(); ++i)
				{
					delete m_StoppedThreads[i];
					m_StoppedThreads[i] = nullptr;
				}
				Log_Assert(LOG_ERR, m_StartingThreads.size() == 0);
				Log_Assert(LOG_ERR, m_StartedThreads.size() == 0);
			}
		}
	
		T* Get()
		{
			Log(LOG_INFO, __FUNCTION__);
			LockGuard<Mutex> mutexLock(m_ThreadsLock);
			if (m_StoppedThreads.size() > 0)
			{
				T* thr = m_StoppedThreads.back();
				m_StoppedThreads.pop_back();
				m_StartingThreads.push_back(thr);
				return thr;
			}
			return nullptr;
		}

		T* Create()
		{
			Log(LOG_INFO, __FUNCTION__);
			T* thr = Get();
			if (thr != nullptr) {
				thr->Create();
			}
			return nullptr;
		}

		void WaitAll()
		{
			Log(LOG_INFO, __FUNCTION__);
			while (true)
			{
				{
					LockGuard<Mutex> mutexLock(m_ThreadsLock);
					if (m_StoppedThreads.size() == m_StoppedThreads.capacity())
					{
						return;
					}
				}
				Sleep(100);
			}
		}

		void JoinAll()
		{
			Log(LOG_INFO, __FUNCTION__);
			LockGuard<Mutex> mutexLock(m_ThreadsLock);
			for (size_t i = 0; i < m_StoppedThreads.size(); ++i)
			{
				m_StoppedThreads[i]->Join();
			}
		}

		void StopAll()
		{
			Log(LOG_INFO, __FUNCTION__);
			LockGuard<Mutex> mutexLock(m_ThreadsLock);
			for (size_t i = 0; i < m_StartedThreads.size(); ++i)
			{
				m_StartedThreads[i]->Stop();
			}
		}
/*
	    T* Get(size_t idx)
	    {
			return m_StoppedThreads[i];
	    }
*/
private:

		virtual void OnStart(Thread* thread) 
		{
			Log(LOG_INFO, "OnStart %lx", thread);
			LockGuard<Mutex> mutexLock(m_ThreadsLock);
			for (size_t i = 0; i < m_StartingThreads.size(); ++i)
			{
				T* threadPtr = m_StartingThreads[i];
				if (threadPtr == thread) 
				{
					m_StartingThreads.erase(m_StartingThreads.begin() + i);
					m_StartedThreads.push_back(threadPtr);
					return;
				}
			}
			Log(LOG_ERR, __FUNCTION__ " thread not found");
		}

		virtual void OnStop(Thread* thread) 
		{ 
			Log(LOG_INFO, "OnStop %lx", thread);
			LockGuard<Mutex> mutexLock(m_ThreadsLock);
			for (size_t i = 0; i < m_StartedThreads.size(); ++i)
			{
				T* threadPtr = m_StartedThreads[i];
				if (threadPtr == thread)
				{
					m_StartedThreads.erase(m_StartedThreads.begin() + i);
					m_StoppedThreads.push_back(threadPtr);
					return;
				}
			}
			Log(LOG_ERR, __FUNCTION__ " thread not found");
		}

		static void ThreadOnStart(Thread::ThreadEventArgs* arg)
		{
			if (arg != nullptr && arg->m_DataPtr != nullptr)
			{
				ThreadPool* self = static_cast<ThreadPool*>(arg->m_DataPtr);
				self->OnStart(arg->m_Thread);
			}
		}

		static void ThreadOnStop(Thread::ThreadEventArgs* arg)
		{
			if (arg != nullptr && arg->m_DataPtr != nullptr)
			{
				ThreadPool* self = static_cast<ThreadPool*>(arg->m_DataPtr);
				self->OnStop(arg->m_Thread);
			}
		}

		Mutex m_ThreadsLock;
	    std::vector<T*> m_StoppedThreads;
		std::vector<T*> m_StartingThreads;
		std::vector<T*> m_StartedThreads;
};

} // namespace thread

#endif // THREADPOOL_H_
