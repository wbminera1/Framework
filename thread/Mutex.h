#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>
#include "../common/Log.h"

namespace thread
{
	
	class Mutex
	{
	    public:
	        Mutex()
	        {
	            pthread_mutex_init(&m_Mutex, NULL);
	        }
	
	        ~Mutex()
	        {
	            pthread_mutex_destroy(&m_Mutex);
	        }
	
	        bool IsLocked()
	        {
	            if (TryLock())
	            {
	                Unlock();
	                return false;
	            }
	            return true;
	        }
	
	        bool TryLock()
	        {
	            int lock = pthread_mutex_trylock(&m_Mutex);
	            return (lock == 0);
	        }
	
	        void Lock()
	        {
	            pthread_mutex_lock(&m_Mutex);
	        }
	
	        void Unlock()
	        {
	            pthread_mutex_unlock(&m_Mutex);
	        }
	
	    protected:
	        pthread_mutex_t& Get()
	        {
	            return  m_Mutex;
	        }
	
	    private:
	        pthread_mutex_t m_Mutex;
	};

	template <class T>
	class LockGuard
	{
		public:
			LockGuard(T& lockable) : m_Lockable(lockable) 
			{ 
				m_Lockable.Lock(); 
				Log(LOG_DEBUG, __FUNCTION__ " locked");
			}
			
			~LockGuard() 
			{ 
				Log(LOG_DEBUG, __FUNCTION__ " unlocked");
				m_Lockable.Unlock();
			}
	private:
		T& m_Lockable;
	};

} // namespace thread


#endif // MUTEX_H_
