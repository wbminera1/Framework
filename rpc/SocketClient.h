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
	SocketClient(sockets::Socket&& sock, const sockets::SocketAddress& addr, CommandDispatcher* dispatcher);
	SocketClient(const sockets::SocketAddress& addr, CommandDispatcher* dispatcher);
	virtual ~SocketClient();

	virtual bool Receive(Command& cmd) { return false; }
	virtual bool Send(const Command& cmd);

	bool Start();

private:
	
	virtual void Process() { Start(); }

	sockets::Socket m_Socket;
	sockets::SocketAddress m_Addr;

	thread::Mutex m_CommandMutex;
	thread::Condition m_CommandWait;
	std::vector<char> m_CommandBuffer;
};

#endif // __SOCKETCLIENT_H_
