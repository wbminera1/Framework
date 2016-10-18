#ifndef SOCKETSERVERBASE_H_
#define SOCKETSERVERBASE_H_

#include <string.h>

#include "../common/Log.h"
#include "../sockets/Socket.h"
#include "../thread/ThreadPool.h"

template <class TClient>
class SocketServerBase : public thread::Thread {
public:
	explicit SocketServerBase(int maxConnections, int port)
		: Thread(__FUNCTION__)
		, m_MaxConnections(maxConnections)
		, m_Port(port)
		, m_Pool(maxConnections)
	{ }

	virtual ~SocketServerBase() { }

protected:

	virtual void OnConnection(TClient* client)
	{
		client->Create();
	}

	virtual void Process()
	{
		sockets::SocketAddress serverA(AF_INET);
		serverA.pton("127.0.0.1");
		serverA.port(m_Port);
		sockets::Socket serverS;

		if (serverS.Open() && serverS.Bind(serverA)) {
			Log(LOG_INFO, __FUNCTION__ " listening");
			while (serverS.Listen(m_MaxConnections)) {
				sockets::Socket connectedS;
				sockets::SocketAddress connectedA(AF_INET);
				if (connectedS.Accept(serverS, connectedA))
				{
					Log(LOG_INFO, __FUNCTION__ " accepted");
					TClient* sc = m_Pool.Get();
					if (sc != nullptr)
					{
						sc->SetConnection(connectedS, connectedA);
						OnConnection(sc);
						Log(LOG_INFO, __FUNCTION__ " client spawned");
					}
					else
					{
						Log(LOG_WARNING, __FUNCTION__ " client not spawned");
					}
				}
			}
		}
	}

	int m_MaxConnections;
	int m_Port;

	thread::ThreadPool<TClient> m_Pool;
};

#endif // SOCKETSERVERBASE_H_