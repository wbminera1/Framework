#ifndef __SOCKETCLIENT_H_
#define __SOCKETCLIENT_H_

#include <vector>
#include <cstdint>

#include "../sockets/Socket.h"

#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"
#include "../thread/Barrier.h"

#include "../sockets/SocketClientBase.h"

#include "Command.h"
#include "CommandDispatcher.h"


class SocketClientRPC : public Dispatched, public SocketClientBase
{
public:
	SocketClientRPC();
	virtual ~SocketClientRPC();

	virtual bool Handle(const Command& cmd, ICommandHandler* source);

	void WaitForStart();

private:
	
	virtual void Process();

	class RecThread : public thread::Thread
	{
	public:
		RecThread(SocketClientRPC* client)
			: Thread(__FUNCTION__)
			, m_Client(client)
		{
			
		}
		void Process();
		SocketClientRPC* m_Client;
	};

	class SendThread : public thread::Thread
	{
	public:
		SendThread(SocketClientRPC* client)
			: Thread(__FUNCTION__)
			, m_Client(client)
		{
			m_CommandBuffer.reserve(1024);
		}

		void Send(const Command& cmd);

	protected:
		virtual void Process();

		SocketClientRPC* m_Client;

		thread::Mutex m_CommandMutex;
		thread::Condition m_CommandWait;
		std::vector<char> m_CommandBuffer;
	};

	RecThread m_RecThread;
	SendThread m_SendThread;

	thread::Barrier m_StartWait;
};

#endif // __SOCKETCLIENT_H_
