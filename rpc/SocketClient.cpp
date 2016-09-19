#include "../common/Log.h"
#include "SocketClient.h"


SocketClient::SocketClient(sockets::Socket& sock, const sockets::SocketAddress& addr, CommandDispatcher* dispatcher)
	: SocketClient(addr, dispatcher)
{
	m_Socket = std::move(sock);
}

SocketClient::SocketClient(const sockets::SocketAddress& addr, CommandDispatcher* dispatcher)
	: Dispatched(dispatcher)
	, m_Addr(addr)
	, m_RecThread(this)
	, m_SendThread(this)
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
	Log(LOG_INFO, __FUNCTION__ " started");
	if (!m_Socket.IsValid()) {
		m_Socket.Open();
		m_Socket.Connect(m_Addr);
	}
	if (!m_Socket.IsValid()) {
		Log(LOG_ERR, "SocketClient - Connect failed with error: %d\n", WSAGetLastError());
		return false;
	}

	Log(LOG_INFO, "SocketClient - connected\n");

	m_RecThread.Create();
	m_SendThread.Create();

	m_RecThread.Join();
	m_SendThread.Join();

	m_Socket.Close();
	Log(LOG_INFO, __FUNCTION__ " stopped");
	return true;
}

