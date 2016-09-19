#ifndef __SOCKETCLIENT_H_
#define __SOCKETCLIENT_H_

#include <vector>
#include <cstdint>

#include "../sockets/Socket.h"

#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"

#include "Command.h"

class CommandDispatcher;

class SocketClient : public Dispatched, public thread::Thread
{
public:
	SocketClient(sockets::Socket& sock, const sockets::SocketAddress& addr, CommandDispatcher* dispatcher);
	SocketClient(const sockets::SocketAddress& addr, CommandDispatcher* dispatcher);
	virtual ~SocketClient();

	virtual bool Receive(Command& cmd) { return false; }
	virtual bool Send(const Command& cmd);

	bool Start();

private:
	
	virtual void Process() { Start(); }

	class RecThread : public thread::Thread
	{
	public:
		RecThread(SocketClient* client)
			: m_Client(client)
		{
			
		}
		virtual void Process() {  
			if (m_Client->m_Socket.IsValid())
			{
				std::vector<char> data;
				while (!m_Stop)
				{
					auto res = m_Client->m_Socket.RecV(data);
					if (!res) {
						Log(LOG_ERR, "RecThread - RecV function failed with error: %d\n", WSAGetLastError());
						break;
					}
					Log(LOG_INFO, "RecThread - RecV %d bytes\n", data.size());
				}
			}
			else
			{
				Log(LOG_ERR, "RecThread - invalid Socket");
			}
		}
		
		SocketClient* m_Client;
	};

	class SendThread : public thread::Thread
	{
	public:
		SendThread(SocketClient* client)
			: m_Client(client)
		{

		}
		virtual void Process() {  
			m_Client->m_CommandWait.Lock();
			while (!m_Stop)
			{
				m_Client->m_CommandWait.Wait();
				m_Client->m_CommandMutex.Lock();
				if (m_Client->m_CommandBuffer.size() > 0)
				{
					auto res = m_Client->m_Socket.Send(m_Client->m_CommandBuffer);
					if (res == SOCKET_ERROR) {
						Log(LOG_ERR, "send function failed with error: %d\n", WSAGetLastError());
						break;
					}
					m_Client->m_CommandBuffer.clear();
					Log(LOG_INFO, "sent %d bytes\n", res);
				}
				m_Client->m_CommandMutex.Unlock();

			}
			m_Client->m_CommandWait.Unlock();
		}

		SocketClient* m_Client;
	};

	sockets::Socket m_Socket;
	sockets::SocketAddress m_Addr;

	RecThread m_RecThread;
	SendThread m_SendThread;

	thread::Mutex m_CommandMutex;
	thread::Condition m_CommandWait;
	std::vector<char> m_CommandBuffer;
};

#endif // __SOCKETCLIENT_H_
