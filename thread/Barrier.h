
#ifndef BARRIER_H_
#define BARRIER_H_

#include <pthread.h>
#include "../common/Log.h"

namespace thread
{
	class Barrier
	{
	public:
		Barrier(unsigned int count)
		{
			pthread_barrier_init(&m_Barrier, nullptr, count);
		}

		~Barrier()
		{
			pthread_barrier_destroy(&m_Barrier);
		}

		void Wait()
		{
			pthread_barrier_wait(&m_Barrier);
		}

	private:

		pthread_barrier_t m_Barrier;
	};

} // namespace thread

#endif // BARRIER_H_