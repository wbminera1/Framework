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

	void ThreadCancelable::Cleanup(void* ptr)
	{
		ThreadCancelable* self = static_cast<ThreadCancelable*>(ptr);
		if (self->m_OnStop.m_Fn != nullptr) {
			(*self->m_OnStop.m_Fn)(&self->m_OnStop.m_Args);
		}
	}


} // namespace thread
