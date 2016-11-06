#include "ThreadBase.h"

namespace thread
{

	ThreadProcReturnType THREADAPI ThreadBase::BaseProc(ThreadProcParamType ptr)
	{
		ThreadBase* self = static_cast<ThreadBase*>(ptr);
		self->OnStart();
		self->Process();
		self->OnStop();
		return 0;
	}

} // namespace thread