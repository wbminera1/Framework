#include "../common/Log.h"
#include "SocketClient.h"


SocketClient::SocketClient(sockets::Socket&& sock, const sockets::SocketAddress& addr, CommandDispatcher* dispatcher)
	: SocketClient(addr, dispatcher)
{
	m_Socket = std::move(sock);
}

SocketClient::SocketClient(const sockets::SocketAddress& addr, CommandDispatcher* dispatcher)
	: Dispatched(dispatcher)
	, m_Addr(addr)
{
	m_CommandBuffer.reserve(1024);
}



SocketClient::~SocketClient()
{
}

bool SocketClient::Send(const Command& cmd)
{
	m_CommandMutex.Lock();
	m_CommandBuffer.clear();
	cmd.Serialize(m_CommandBuffer);
	m_CommandWait.Lock();
	m_CommandWait.Signal();
	m_CommandWait.Unlock();
	m_CommandMutex.Unlock();
	return true;
}

bool SocketClient::Start()
{
	if (!m_Socket.IsValid()) {
		m_Socket.Open();
		m_Socket.Connect(m_Addr);
	}
	if (!m_Socket.IsValid()) {
		Log(LOG_ERR, "SocketClient - Connect failed with error: %d\n", WSAGetLastError());
		return false;
	}

	Log(LOG_INFO, "SocketClient - connected\n");

	m_CommandWait.Lock();
	while (!m_Stop)
	{
		m_CommandWait.Wait();
		m_CommandMutex.Lock();
		if (m_CommandBuffer.size() > 0)
		{
			auto res = m_Socket.Send(m_CommandBuffer);
			if (res == SOCKET_ERROR) {
				Log(LOG_ERR, "send function failed with error: %d\n", WSAGetLastError());
				break;
			}
			m_CommandBuffer.clear();
			Log(LOG_INFO, "sent %d bytes\n", res);
		}
		m_CommandMutex.Unlock();
	}
	m_CommandWait.Unlock();

	m_Socket.Close();
	return true;
}

