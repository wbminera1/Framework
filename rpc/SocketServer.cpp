#include <string.h>

#include "../sockets/Socket.h"

#include "../common/Log.h"
#include "SocketServer.h"
#include "SocketClient.h"
#include "Command.h"

SocketServer::SocketServer(int port, CommandDispatcher* dispatcher)
	: Thread(__FUNCTION__)
	, m_Dispatcher(dispatcher)
  , m_Port(port)
  , m_Pool(MAX_CONNECTIONS)
{
}

SocketServer::~SocketServer()
{
}

bool SocketServer::Start()
{
	sockets::SocketAddress serverA(AF_INET);
	serverA.pton("127.0.0.1");
	serverA.port(m_Port);
	sockets::Socket serverS;

	if (serverS.Open() && serverS.Bind(serverA)) {
		Log(LOG_INFO, "SocketServer - listening");
		while (serverS.Listen(MAX_CONNECTIONS)) {
			sockets::Socket connectedS;
			sockets::SocketAddress connectedA(AF_INET);
			if (connectedS.Accept(serverS, connectedA))
			{
				Log(LOG_INFO, "SocketServer - accepted");
				SocketClient* sc = m_Pool.Get();
				if (sc != nullptr)
				{
					sc->SetConnection(connectedS, connectedA, m_Dispatcher);
					sc->Create();
					sc->WaitForStart();
				}
				Log(LOG_WARNING, "SocketServer - client spawned");
			}
		}
	}
	return false;
}


