#ifndef __SOCKETCLIENT_H_
#define __SOCKETCLIENT_H_

#include <vector>
#include <cstdint>

#include "../sockets/Socket.h"

#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"

#include "Command.h"
#include "CommandDispatcher.h"

class SocketClient : public Dispatched, public thread::Thread
{
public:
	SocketClient(sockets::Socket& sock, const sockets::SocketAddress& addr, CommandDispatcher* dispatcher);
	SocketClient(const sockets::SocketAddress& addr, CommandDispatcher* dispatcher);
	virtual ~SocketClient();

	virtual bool Receive(const Command& cmd) { return false; }
	virtual bool Send(const Command& cmd);

	bool Start();
	void Stop();

private:
	
	virtual void Process() { Start(); }

	class RecThread : public thread::Thread
	{
	public:
		RecThread(SocketClient* client)
			: m_Client(client)
		{
			
		}
		void Process();
		SocketClient* m_Client;
	};

	class SendThread : public thread::Thread
	{
	public:
		SendThread(SocketClient* client)
			: m_Client(client)
		{

		}
		virtual void Process();

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
