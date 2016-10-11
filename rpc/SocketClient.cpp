#include "../common/Log.h"
#include "SocketClient.h"


SocketClient::SocketClient()
	: Dispatched(nullptr)
	, Thread(__FUNCTION__)
	, m_Addr(0)
	, m_RecThread(this)
	, m_SendThread(this)
	, m_StartWait(4)
{
	m_CommandBuffer.reserve(1024);
	Log(LOG_WARNING, __FUNCTION__);
}

SocketClient::~SocketClient()
{
	Log(LOG_WARNING, __FUNCTION__ );
}

void SocketClient::SetConnection(sockets::Socket& sock, const sockets::SocketAddress& addr, CommandDispatcher* dispatcher)
{
	m_Socket = std::move(sock);
	m_Addr = addr;
	SetDispatcher(dispatcher);
}

void SocketClient::SetAddr(const sockets::SocketAddress& addr, CommandDispatcher* dispatcher)
{
	m_Addr = addr;
	SetDispatcher(dispatcher);
}


bool SocketClient::Handle(const Command& cmd, ICommandHandler* source)
{
	Log(LOG_INFO, __FUNCTION__ " started");
	thread::LockGuard<thread::Mutex> cmdLock(m_CommandMutex);
	{
		thread::LockGuard<thread::Condition> cmdLock(m_CommandWait);
		m_CommandBuffer.clear();
		cmd.Serialize(m_CommandBuffer);
		m_CommandWait.Signal();
	}
	Log(LOG_INFO, __FUNCTION__ " stopped");
	return true;
}


void SocketClient::Process()
{
	Log(LOG_INFO, __FUNCTION__ " started");
	if (!m_Socket.IsValid()) {
		m_Socket.Open();
		m_Socket.Connect(m_Addr);
	}
	if (m_Socket.IsValid()) {
		Log(LOG_INFO, "SocketClient - connected");


		m_RecThread.Create();
		m_SendThread.Create();

		m_StartWait.Wait();

		Thread::Process();

		m_RecThread.Stop();
		m_SendThread.Stop();

		{
			thread::LockGuard<thread::Condition> cmdLock(m_CommandWait);
			m_CommandWait.Signal();
		}

		m_RecThread.Join();
		m_SendThread.Join();
	}
	else
	{
		Log(LOG_ERR, "SocketClient - Connect failed with error: %d", WSAGetLastError());
	}
	Log(LOG_INFO, __FUNCTION__ " stopped");
}

void SocketClient::WaitForStart()
{
	m_StartWait.Wait();
}

void SocketClient::Stop()
{
	Log(LOG_INFO, __FUNCTION__ " started");

	Thread::Stop();
	m_Socket.Close();

	Log(LOG_INFO, __FUNCTION__ " stopped");
}

void SocketClient::RecThread::Process() {
	Log(LOG_INFO, __FUNCTION__ " started");
	if (m_Client->m_Socket.IsValid())
	{
		std::vector<char> data;
		m_Client->m_StartWait.Wait();

		while (!m_Stop)
		{
			auto res = m_Client->m_Socket.RecV(data, Command::MAX_COMMAND_SIZE);
			if (!res) {
				Log(LOG_ERR, "RecThread - RecV function failed with error: %d", WSAGetLastError());
				m_Client->Stop();
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
				if (cmd != NULL && m_Client->GetDispatcher() != nullptr) {
					m_Client->GetDispatcher()->DispatchFrom(*cmd, m_Client);
				}
			}
		}
	}
	else
	{
		Log(LOG_ERR, "RecThread - invalid Socket");
	}
	Log(LOG_INFO, __FUNCTION__ " stopped");
}

void SocketClient::SendThread::Process() {
	Log(LOG_INFO, __FUNCTION__ " started");
	thread::LockGuard<thread::Condition> cmdLock(m_Client->m_CommandWait);
	m_Client->m_StartWait.Wait();
	while (!m_Stop)
	{
		Log(LOG_INFO, __FUNCTION__ " on wait");
		m_Client->m_CommandWait.Wait();
		{
			thread::LockGuard<thread::Mutex> cmdLock(m_Client->m_CommandMutex);
			if (m_Client->m_CommandBuffer.size() > 0)
			{
				auto res = m_Client->m_Socket.Send(m_Client->m_CommandBuffer);
				if (res == SOCKET_ERROR) {
					Log(LOG_ERR, __FUNCTION__ "send function failed with error: %d", WSAGetLastError());
					m_Client->Stop();
					break;
				}
				m_Client->m_CommandBuffer.clear();
				Log(LOG_INFO, __FUNCTION__ "sent %d bytes", res);
			}
			else
			{
				Log(LOG_WARNING, __FUNCTION__ " triggered, but commandBuffer is empty");
			}
		}
	}
	Log(LOG_INFO, __FUNCTION__ " stopped");
}
