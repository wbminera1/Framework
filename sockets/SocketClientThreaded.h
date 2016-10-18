#ifndef SOCKETCLIENTTHREADED_H_
#define SOCKETCLIENTTHREADED_H_
#include "../common/Log.h"
#include "../sockets/Socket.h"
#include "../thread/Condition.h"
#include "../thread/Barrier.h"
#include "../sockets/SocketClientBase.h"
#include "../sockets/SocketServerBase.h"
#include "../sockets/SocketRecThread.h"
#include "../sockets/SocketSendThread.h"

namespace sockets
{

template <class TRecThread, class TSendThread>
class SocketClientThreaded : public SocketClientBase, public thread::ThreadEventListener
{
public:
	SocketClientThreaded() : m_OnStartSync(3)
	{ }

protected:

	virtual void OnStart()
	{
		Listen(&m_RecThread);
		Listen(&m_SendThread);
		m_RecThread.SetOnReceived(ThreadOnReceived, this);
		m_RecThread.SetSocket(&GetSocket());
		m_SendThread.SetSocket(&GetSocket());
		m_RecThread.Create();
		m_SendThread.Create();
		m_OnStartSync.Wait();
	}

	virtual void Process()
	{
		Log(LOG_DEBUG, __FUNCTION__ " started");

		if (m_Socket.IsValid()) {
			Log(LOG_INFO, __FUNCTION__ " connected");

			SocketClientBase::Process();

		}
		else
		{
			Log(LOG_ERR, __FUNCTION__ " invalid socket");
		}
		Log(LOG_DEBUG, __FUNCTION__ " stopped");
	}

	virtual void OnStop()
	{
		m_RecThread.Stop();
		m_SendThread.Stop();

		m_RecThread.Join();
		m_SendThread.Join();
	}

	virtual void OnStartEvent(Thread* thread)
	{
		Log(LOG_DEBUG, __FUNCTION__ " started %s", thread->GetName().c_str());
		m_OnStartSync.Wait();
		Log(LOG_DEBUG, __FUNCTION__ " stopped %s", thread->GetName().c_str());
	}

	virtual void OnStopEvent(Thread* thread)
	{
		Log(LOG_DEBUG, __FUNCTION__ " started %s", thread->GetName().c_str());
		Stop();
		Log(LOG_DEBUG, __FUNCTION__ " stopped %s", thread->GetName().c_str());
	}

	virtual void OnReceivedEvent(Thread* thread)
	{
		Log(LOG_DEBUG, __FUNCTION__ " started %s", thread->GetName().c_str());
		Log(LOG_DEBUG, __FUNCTION__ " data size %d", m_RecThread.GetData().size());
		m_SendThread.Send(m_RecThread.GetData());
		Log(LOG_DEBUG, __FUNCTION__ " stopped %s", thread->GetName().c_str());
	}

	static void ThreadOnReceived(Thread::ThreadEventArgs* arg)
	{
		if (arg != nullptr && arg->m_DataPtr != nullptr)
		{
			SocketClientThreaded* self = static_cast<SocketClientThreaded*>(arg->m_DataPtr);
			self->OnReceivedEvent(arg->m_Thread);
		}
	}


	TRecThread  m_RecThread;
	TSendThread m_SendThread;
	thread::Barrier  m_OnStartSync;
};

} //namespace sockets

#endif // SOCKETCLIENTTHREADED_H_