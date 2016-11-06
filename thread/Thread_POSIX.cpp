#include "Thread.h"
namespace thread
{

	ThreadProcReturnType THREADAPI ThreadCancelable::CancelableProc(ThreadProcParamType ptr)
	{
		ThreadCancelable* self = static_cast<ThreadCancelable*>(ptr);
		pthread_cleanup_push(Cleanup, self);
		self->OnStart();
		self->Process();
		pthread_cleanup_pop(1);
		return NULL;
	}


} // namespace thread
