#include <windows.h>
#include <excpt.h>

#include "Thread.h"
namespace thread
{

	void Thread::Sleep(unsigned long milliseconds)
	{
		::Sleep(milliseconds);
	}

	void Thread::Process()
	{
	}

	void Thread::OnStart()
	{
		ThreadBase::OnStart();
		if (m_OnStart.m_Fn != nullptr) {
			(*m_OnStart.m_Fn)(&m_OnStart.m_Args);
		}
	}

	void Thread::OnStop()
	{
		ThreadBase::OnStop();
		if (m_OnStop.m_Fn != nullptr) {
			(*m_OnStop.m_Fn)(&m_OnStop.m_Args);
		}

	}

	int ThreadCancelable::Create()
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr); // TODO attributes to class
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		int res = pthread_create(&m_Thread, &attr, &ThreadCancelable::StartCancelable, (void *) this);
		pthread_attr_destroy(&attr);
		return res;
	}

	void* ThreadCancelable::StartCancelable(void* ptr)
	{
		ThreadCancelable* self = static_cast<ThreadCancelable*>(ptr);
		pthread_cleanup_push(Cleanup, self);
		self->OnStart();
		self->Process();
		pthread_cleanup_pop(1);
		return NULL;
	}

	void ThreadCancelable::Cleanup(void* ptr)
	{
		ThreadCancelable* self = static_cast<ThreadCancelable*>(ptr);
		if (self->m_OnStop.m_Fn != nullptr) {
			(*self->m_OnStop.m_Fn)(&self->m_OnStop.m_Args);
		}
	}


} // namespace thread
