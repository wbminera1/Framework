#include <windows.h>
#include <excpt.h>

#include "Thread.h"
namespace thread
{

	ThreadProcReturnType THREADAPI ThreadCancelable::CancelableProc(ThreadProcParamType ptr)
	{
		ThreadCancelable* self = static_cast<ThreadCancelable*>(ptr);
		{
		__try
			{
			self->OnStart();
			self->Process();
			}
        __finally
            {
            if(AbnormalTermination())
				{
                }
			Cleanup(ptr);
            }
        }
		return NULL;
	}


} // namespace thread
