#include "SocketSendThread.h"

namespace sockets
{
	bool SocketSendThread::Create()
	{
		thread::LockGuard<thread::Condition> cmdLock(m_CommandWait);
		return thread::Thread::Create();
	}

	void SocketSendThread::Process()
	{
		Log(LOG_DEBUG, __FUNCTION__ " started");
		while (!m_Stop)
		{
			Log(LOG_INFO, __FUNCTION__ " on wait");
			m_CommandWait.Wait();
			{
				if (m_Data.size() > 0)
				{
					auto res = m_Socket->Send(m_Data);
					if (res == SOCKET_ERROR) {
						Log(LOG_ERR, __FUNCTION__ " send function failed with error: %d", WSAGetLastError());
						Stop();
						break;
					}
					m_Data.erase(m_Data.begin(), m_Data.begin() + res);
					Log(LOG_INFO, __FUNCTION__ " sent %d bytes", res);
				}
				else
				{
					Log(LOG_WARNING, __FUNCTION__ " triggered, but data is empty");
				}
			}
		}
		Log(LOG_DEBUG, __FUNCTION__ " stopped");
	}

	void SocketSendThread::Stop()
	{
		Thread::Stop();
		m_CommandWait.Signal();
	}

	void SocketSendThread::Send(const std::vector<char>& data)
	{
		{
			thread::LockGuard<thread::Condition> cmdLock(m_CommandWait);
			m_Data.insert(m_Data.end(), data.begin(), data.end());
		}
		m_CommandWait.Signal();
	}

} // namespace sockets



