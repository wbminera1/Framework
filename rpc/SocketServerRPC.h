#ifndef SOCKETLISTENER_H_
#define SOCKETLISTENER_H_

#include "../sockets/SocketServerBase.h"
#include "CommandDispatcher.h"

template <class TClient>
class SocketServerRPC : public SocketServerBase<TClient>
{
public:
	explicit SocketServerRPC(int maxConnections, int port, CommandDispatcher* dispatcher)
		: SocketServerBase(maxConnections, port)
		, m_Dispatcher(dispatcher)
	{ }

	virtual ~SocketServerRPC() { }

protected:

	virtual void OnConnection(TClient* client)
	{
		client->SetDispatcher(m_Dispatcher);
		client->Create();
		//client->WaitForStart();
	}

	CommandDispatcher* m_Dispatcher;
};

#endif /* SOCKETLISTENER_H_ */
