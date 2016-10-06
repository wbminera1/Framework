#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <vector>
#include "Thread.h"

namespace thread
{

template <class T>
class ThreadPool
{
	public:

		ThreadPool(size_t size)
	    {
	        m_Threads.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				T* threadPtr = new T();
				threadPtr->SetOnStart(ThreadOnStart, this);
				threadPtr->SetOnStop(ThreadOnStop, this);
				m_Threads.push_back(threadPtr);
			}
		}
	
	    virtual ~ThreadPool()
	    {
			Stop();
			Sleep(1000);
			Join();
			for (size_t i = 0; i < m_Threads.size(); ++i)
			{
				delete m_Threads[i];
				m_Threads[i] = nullptr;
			}
		}
	
		void Create()
		{
			for (size_t i = 0; i < m_Threads.size(); ++i)
			{
				m_Threads[i]->Create();
			}
		}
		
		void Join()
		{
			for (size_t i = 0; i < m_Threads.size(); ++i)
			{
				m_Threads[i]->Join();
			}
		}

		void Stop()
		{
			for (size_t i = 0; i < m_Threads.size(); ++i)
			{
				m_Threads[i]->Stop();
			}
		}

	    T* Get(size_t idx)
	    {
			return m_Threads[i];
	    }
	
		virtual void OnStart(Thread* thread) { }
		virtual void OnStop(Thread* thread) { }

	private:
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

	    std::vector<T*> m_Threads;
};

} // namespace thread

#endif // THREADPOOL_H_
