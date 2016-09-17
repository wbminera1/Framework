#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <vector>

namespace thread
{

template <class T>
class ThreadPool
{
	public:
	    ThreadPool(size_t size)
	    {
	        m_Threads.reserve(size);
	    }
	
	    ~ThreadPool()
	    {
			//!!
	    }
	
		void Create()
		{
			for (size_t i = 0; i < m_Threads.capacity(); ++i)
			{
				T* threadPtr = new T();
				threadPtr->Create();
				m_Threads.push_back(threadPtr);
			}
		}
		
		void Join()
		{
			for (size_t i = 0; i < m_Threads.capacity(); ++i)
			{
				m_Threads[i]->Join();
			}
		}

		void Stop()
		{
			for (size_t i = 0; i < m_Threads.capacity(); ++i)
			{
				m_Threads[i]->Stop();
			}
		}

	    T* Get(size_t idx)
	    {
			return m_Threads[i];
	    }
	
	private:
	    std::vector<T*> m_Threads;
};

} // namespace thread

#endif // THREADPOOL_H_
