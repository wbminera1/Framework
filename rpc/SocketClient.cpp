#include "../common/Log.h"
#include "SocketClient.h"


void SocketClientBase::OnCreate()
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	if (!m_Socket.IsValid()) {
		m_Socket.Open();
		m_Socket.Connect(m_Addr);
	}

	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}

void SocketClientBase::Process()
{
}

SocketClient::SocketClient()
	: Dispatched(nullptr)
	, m_RecThread(this)
	, m_SendThread(this)
	, m_StartWait(4)
{
	Log(LOG_WARNING, __FUNCTION__);
}

SocketClient::~SocketClient()
{
	Log(LOG_WARNING, __FUNCTION__ );
}

void SocketClientBase::SetConnection(sockets::Socket& sock, const sockets::SocketAddress& addr)
{
	m_Socket = std::move(sock);
	m_Addr = addr;
}

void SocketClientBase::SetAddr(const sockets::SocketAddress& addr)
{
	m_Addr = addr;
}


bool SocketClient::Handle(const Command& cmd, ICommandHandler* source)
{
	Log(LOG_DEBUG, __FUNCTION__ " started");
	
	m_SendThread.Send(cmd);

	Log(LOG_DEBUG, __FUNCTION__ " stopped");
	return false;
}


void SocketClient::Process()
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	SocketClientBase::Process();

	if (m_Socket.IsValid()) {
		Log(LOG_INFO, __FUNCTION__ " connected");


		m_RecThread.Create();
		m_SendThread.Create();

		m_StartWait.Wait();

		Thread::Process();

		m_RecThread.Stop();
		m_SendThread.Stop();

		Command cmdExit(cExit);
		m_SendThread.Send(cmdExit);

		m_RecThread.Join();
		m_SendThread.Join();
	}
	else
	{
		Log(LOG_ERR, __FUNCTION__ " Connect failed with error: %d", WSAGetLastError());
	}
	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}

void SocketClient::WaitForStart()
{
	m_StartWait.Wait();
}

void SocketClientBase::Stop()
{
	Log(LOG_DEBUG, __FUNCTION__ " started");

	Thread::Stop();
	m_Socket.Close();

	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}

void SocketClient::SendThread::Send(const Command& cmd)
{
	thread::LockGuard<thread::Mutex> cmdLock(m_CommandMutex);
	{
		thread::LockGuard<thread::Condition> cmdLock(m_CommandWait);
		m_CommandBuffer.clear();
		cmd.Serialize(m_CommandBuffer);
		m_CommandWait.Signal();
	}
}

void SocketClient::RecThread::Process() {
	Log(LOG_DEBUG, __FUNCTION__ " started");
	if (m_Client->GetSocket().IsValid())
	{
		std::vector<char> data;
		m_Client->WaitForStart();

		while (!m_Stop)
		{
			auto res = m_Client->GetSocket().RecV(data, Command::MAX_COMMAND_SIZE);
			if (!res) {
				Log(LOG_ERR, __FUNCTION__ " RecV function failed with error: %d", WSAGetLastError());
				m_Client->Stop();
				break;
			}
			Log(LOG_INFO, __FUNCTION__ " RecV %d bytes", data.size());
			uint32_t* dataSize = (uint32_t*)&data[0];
			if (*dataSize > Command::MAX_COMMAND_SIZE) {
				Log(LOG_ERR, __FUNCTION__ " wrong data size: %d", *dataSize);
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
		Log(LOG_ERR, __FUNCTION__ " invalid Socket");
	}
	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}

void SocketClient::SendThread::Process() {
	Log(LOG_DEBUG, __FUNCTION__ " started");
	thread::LockGuard<thread::Condition> cmdLock(m_CommandWait);
	m_Client->WaitForStart();
	while (!m_Stop)
	{
		Log(LOG_INFO, __FUNCTION__ " on wait");
		m_CommandWait.Wait();
		{
			thread::LockGuard<thread::Mutex> cmdLock(m_CommandMutex);
			if (m_CommandBuffer.size() > 0)
			{
				auto res = m_Client->m_Socket.Send(m_CommandBuffer);
				if (res == SOCKET_ERROR) {
					Log(LOG_ERR, __FUNCTION__ " send function failed with error: %d", WSAGetLastError());
					m_Client->Stop();
					break;
				}
				m_CommandBuffer.clear();
				Log(LOG_INFO, __FUNCTION__ " sent %d bytes", res);
			}
			else
			{
				Log(LOG_WARNING, __FUNCTION__ " triggered, but commandBuffer is empty");
			}
		}
	}
	Log(LOG_DEBUG, __FUNCTION__ " stopped");
}
