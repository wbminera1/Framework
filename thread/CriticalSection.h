#ifndef CRITICALSECTION_H_
#define CRITICALSECTION_H_
#ifdef _WIN64
#include <windows.h>
typedef CRITICAL_SECTION   CriticalSectionType;
#endif
#ifdef __unix__
#include <pthread.h>
typedef pthread_cond_t CriticalSectionType;
#endif

#include "../common/Log.h"

namespace thread
{
	
	class CriticalSection
	{
	    public:
			CriticalSection();
			~CriticalSection();
	
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

			CriticalSectionType& Get()
	        {
	            return  m_CriticalSection;
	        }
	
	    private:
			CriticalSectionType m_CriticalSection;
	};

} // namespace thread


#endif // CRITICALSECTION_H_
