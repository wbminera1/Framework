#include "ThreadBase.h"

}


namespace thread
{
	void ThreadBase::OnStart()
	{
		m_Stop = false;
	}

	int ThreadBase::Create()
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr); // TODO attributes to class
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		int res = pthread_create(&m_Thread, &attr, &ThreadBase::Start, (void *) this);
		pthread_attr_destroy(&attr);
		return res != NULL;
	}

	int ThreadBase::Join()
	{
		void* status;
		int res = pthread_join(m_Thread, &status);
		return res;
	}


} // namespace thread