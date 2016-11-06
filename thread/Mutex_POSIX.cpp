#include "Mutex.h"

#include "Mutex.h"
namespace thread
{

	Mutex::Mutex()
	{
		pthread_mutex_init(&m_Mutex, NULL);
	}

	Mutex::~Mutex()
	{
		pthread_mutex_destroy(&m_Mutex);
	}


	bool Mutex::TryLock()
	{
		int lock = pthread_mutex_trylock(&m_Mutex);
		return (lock == 0);
	}

	void Mutex::Lock()
	{
		pthread_mutex_lock(&m_Mutex);
	}

	void Mutex::Unlock()
	{
		pthread_mutex_unlock(&m_Mutex);
	}

	pthread_mutex_t& Mutex::Get()
	{
		return  m_Mutex;
	}

} // namespace thread
