#ifndef SOCKETLISTENER_H_
#define SOCKETLISTENER_H_

#include "../thread/Thread.h"

#include "Command.h"
#include "CommandDispatcher.h"

class SocketServer : public thread::Thread {
public:
	explicit SocketServer(int port, CommandDispatcher* dispatcher);
	virtual ~SocketServer();

	bool Start();

private:
	static const int MAX_CONNECTIONS = 16;

	virtual void Process() { Start(); }

	int m_Port;
	CommandDispatcher* m_Dispatcher;
};

#endif /* SOCKETLISTENER_H_ */
