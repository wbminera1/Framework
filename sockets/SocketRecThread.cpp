#include "SocketRecThread.h"

namespace sockets
{
	void SocketRecThread::Process() {
		Log(LOG_DEBUG, __FUNCTION__ " started");
		if (m_Socket->IsValid())
		{
			while (!m_Stop)
			{
				auto res = m_Socket->RecV(m_Data, m_BufferSize);
				if (res) {
					Log(LOG_INFO, __FUNCTION__ " RecV %d bytes", m_Data.size());
					if (m_OnReceived.m_Fn != nullptr) {
						(*m_OnReceived.m_Fn)(&m_OnReceived.m_Args);
					}
					m_Data.clear();
				}
				else
				{
					Log(LOG_ERR, __FUNCTION__ " RecV function failed with error: %d", WSAGetLastError());
					break;
				}
			}
		}
		else
		{
			Log(LOG_ERR, __FUNCTION__ " invalid Socket");
		}
		Log(LOG_DEBUG, __FUNCTION__ " stopped");
	}

} // namespace sockets

