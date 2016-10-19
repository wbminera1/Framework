#ifndef __SOCKETCLIENT_H_
#define __SOCKETCLIENT_H_

#include <vector>
#include <cstdint>

#include "../sockets/Socket.h"

#include "../thread/Thread.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"
#include "../thread/Barrier.h"

#include "../sockets/SocketClientThreaded.h"

#include "Command.h"
#include "CommandDispatcher.h"


class SocketClientRPC : public Dispatched,  public sockets::SocketClientThreaded<sockets::SocketRecThread, sockets::SocketSendThread>
{
public:
	SocketClientRPC();
	virtual ~SocketClientRPC();

	virtual bool Handle(const Command& cmd, ICommandHandler* source);


private:
	
	virtual void Process();

	virtual void OnReceivedEvent(Thread* thread);

	std::vector<char> m_CommandBuffer;
};

#endif // __SOCKETCLIENT_H_
