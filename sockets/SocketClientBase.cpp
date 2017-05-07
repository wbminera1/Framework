#include "SocketClientBase.h"

void SocketClientBase::SetConnection(sockets::Socket& sock, const sockets::SocketAddress& addr)
{
	m_Socket = std::move(sock);
	m_Addr = addr;
}

void SocketClientBase::SetAddr(const sockets::SocketAddress& addr)
{
	m_Addr = addr;
}

void SocketClientBase::OnStart()
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	Thread::OnStart();

	if (!m_Socket.IsValid()) {
		m_Socket.Open();
		m_Socket.Connect(m_Addr);
	}

	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}

void SocketClientBase::Process()
{
	while (!m_Stop)
	{
		if (OnIdle()) {
			thread::Thread::Sleep(100);
		}
	}
}

bool SocketClientBase::OnIdle()
{
	return true;
}


void SocketClientBase::Stop()
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	Thread::Stop();
	m_Socket.Close();

	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}




