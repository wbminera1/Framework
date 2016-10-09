#ifndef SOCKETLISTENER_H_
#define SOCKETLISTENER_H_

#include "../thread/ThreadPool.h"

#include "Command.h"
#include "CommandDispatcher.h"

class SocketClient;

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
	thread::ThreadPool<SocketClient> m_Pool;
};

#endif /* SOCKETLISTENER_H_ */
