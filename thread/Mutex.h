#ifndef MUTEX_H_
#define MUTEX_H_
#ifdef _WIN64
#include <windows.h>
typedef HANDLE  MutexType;
#endif
#ifdef __unix__
#include <pthread.h>
typedef pthread_mutex_t MutexType;
#endif

#include "../common/Log.h"

namespace thread
{
	
	class Mutex
	{
	    public:
			Mutex();
			~Mutex();
	
	        bool IsLocked()
	        {
	            if (TryLock())
	            {
	                Unlock();
	                return false;
	            }
	            return true;
	        }
	
			bool TryLock();
			void Lock();
			void Unlock();
	
	    protected:

			MutexType& Get()
	        {
	            return  m_Mutex;
	        }
	
	    private:
			MutexType m_Mutex;
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
