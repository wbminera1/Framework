#ifndef CONDITION_H_
#define CONDITION_H_
#include "Mutex.h"
#include "Misc.h"

namespace thread
{
	
class Condition : public Mutex
{
	public:
	    Condition()
	        : m_Condition(PTHREAD_COND_INITIALIZER)
	    {
	    }
	
	    ~Condition()
	    {
	        pthread_cond_destroy(&m_Condition);
	    }
	
	    int Wait()
	    {
	        return pthread_cond_wait(&m_Condition, &Get());
	    }
	
	    int TimedWait(time_t sec = 0, long nsec = 0)
	    {
	        timespec now, timeout;
	        gettimeofday(&now);
	        timeout.tv_sec = now.tv_sec + sec;
	        timeout.tv_nsec = now.tv_nsec * nsec;
	        return pthread_cond_timedwait(&m_Condition, &Get(), &timeout);
	    }
	
	    int Signal()
	    {
	        return pthread_cond_signal(&m_Condition);
	    }
	
	    int Broadcast()
	    {
	        return pthread_cond_broadcast(&m_Condition);
	    }
	
	private:
	    pthread_cond_t m_Condition;
};

} // namespace thread

#endif // CONDITION_H_
