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
		Log(LOG_ERR, "SocketClient - Connect failed with error: %d", WSAGetLastError());
		return false;
	}

	Log(LOG_INFO, "SocketClient - connected");

	m_RecThread.Create();
	m_SendThread.Create();

	Log(LOG_INFO, __FUNCTION__ " stopped");
	return true;
}

void SocketClient::Stop()
{
	Log(LOG_INFO, __FUNCTION__ " started");

	m_RecThread.Join();
	m_SendThread.Join();

	m_Socket.Close();

	Log(LOG_INFO, __FUNCTION__ " stopped");
}

void SocketClient::RecThread::Process() {
	if (m_Client->m_Socket.IsValid())
	{
		std::vector<char> data;
		while (!m_Stop)
		{
			auto res = m_Client->m_Socket.RecV(data, Command::MAX_COMMAND_SIZE);
			if (!res) {
				Log(LOG_ERR, "RecThread - RecV function failed with error: %d", WSAGetLastError());
				break;
			}
			Log(LOG_INFO, "RecThread - RecV %d bytes", data.size());
			uint32_t* dataSize = (uint32_t*)&data[0];
			if (*dataSize > Command::MAX_COMMAND_SIZE) {
				Log(LOG_ERR, "RecThread - wrong data size: %d", *dataSize);
				data.clear();
				continue;
			}
			if (data.size() >= *dataSize) {
				Command* cmd = Command::Create(data);
				if (cmd != NULL && m_Client->m_Dispatcher != nullptr) {
					m_Client->m_Dispatcher->Dispatch(*cmd, m_Client);
				}
			}
		}
	}
	else
	{
		Log(LOG_ERR, "RecThread - invalid Socket");
	}
}

void SocketClient::SendThread::Process() {
	m_Client->m_CommandWait.Lock();
	while (!m_Stop)
	{
		m_Client->m_CommandWait.Wait();
		m_Client->m_CommandMutex.Lock();
		if (m_Client->m_CommandBuffer.size() > 0)
		{
			auto res = m_Client->m_Socket.Send(m_Client->m_CommandBuffer);
			if (res == SOCKET_ERROR) {
				Log(LOG_ERR, "send function failed with error: %d", WSAGetLastError());
				break;
			}
			m_Client->m_CommandBuffer.clear();
			Log(LOG_INFO, "sent %d bytes", res);
		}
		m_Client->m_CommandMutex.Unlock();

	}
	m_Client->m_CommandWait.Unlock();
}
