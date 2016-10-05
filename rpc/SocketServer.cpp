#include <string.h>

#include "../sockets/Socket.h"

#include "../common/Log.h"
#include "SocketServer.h"
#include "SocketClient.h"
#include "Command.h"

SocketServer::SocketServer(int port, CommandDispatcher* dispatcher)
  : m_Dispatcher(dispatcher)
  , m_Port(port)
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
				SocketClient* sc = new SocketClient(connectedS, connectedA, m_Dispatcher);
				sc->Start();
				Log(LOG_WARNING, "SocketServer - client spawned");
			}
		}
	}
	return false;
}


