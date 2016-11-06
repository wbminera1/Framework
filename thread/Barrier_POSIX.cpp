#include "../common/Log.h"

#include "Barrier.h"

namespace thread
{
	Barrier::Barrier(unsigned int count)
	{
		pthread_barrier_init(&m_Barrier, nullptr, count);
	}

	Barrier::~Barrier()
	{
		pthread_barrier_destroy(&m_Barrier);
	}

	void Barrier::Wait()
	{
		pthread_barrier_wait(&m_Barrier);
	}

} // namespace thread


