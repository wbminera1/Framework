#ifndef __SOCKETCLIENT_H_
#define __SOCKETCLIENT_H_

#include <vector>
#include <cstdint>

#include "../sockets/Socket.h"

#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"
#include "../thread/Barrier.h"

#include "Command.h"
#include "CommandDispatcher.h"

class SocketClientBase : public thread::Thread
{
public:
	SocketClientBase() : Thread(__FUNCTION__) { }
	virtual ~SocketClientBase() {}

	void SetConnection(sockets::Socket& sock, const sockets::SocketAddress& addr);
	void SetAddr(const sockets::SocketAddress& addr);
	
	sockets::Socket& GetSocket() { return m_Socket; }

	virtual void Stop();

protected:

	virtual void OnStart();
	virtual void Process();

	sockets::Socket m_Socket;
	sockets::SocketAddress m_Addr;

};

template <class TRecThread, class TSendThread>
class SocketClientThreaded : public SocketClientBase
{
public:
protected:
	TRecThread  m_RecThread;
	TSendThread m_SendThread;
};

class SocketClient : public Dispatched, public SocketClientBase
{
public:
	SocketClient();
	virtual ~SocketClient();

	virtual bool Handle(const Command& cmd, ICommandHandler* source);

	void WaitForStart();

private:
	
	virtual void Process();

	class RecThread : public thread::Thread
	{
	public:
		RecThread(SocketClient* client)
			: Thread(__FUNCTION__)
			, m_Client(client)
		{
			
		}
		void Process();
		SocketClient* m_Client;
	};

	class SendThread : public thread::Thread
	{
	public:
		SendThread(SocketClient* client)
			: Thread(__FUNCTION__)
			, m_Client(client)
		{
			m_CommandBuffer.reserve(1024);
		}

		void Send(const Command& cmd);

	protected:
		virtual void Process();

		SocketClient* m_Client;

		thread::Mutex m_CommandMutex;
		thread::Condition m_CommandWait;
		std::vector<char> m_CommandBuffer;
	};

	RecThread m_RecThread;
	SendThread m_SendThread;

	thread::Barrier m_StartWait;
};

#endif // __SOCKETCLIENT_H_
