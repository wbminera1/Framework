#include "../common/Log.h"

#include "SocketServer.h"
#include "SocketClient.h"


int Server_Test()
{
	Log(LOG_INFO, __FUNCTION__ " started");
	sockets::Socket::Startup();

	CommandDispatcher dispatcher;

	SocketServer sl(9999, &dispatcher);
	sl.Create();

	while (true)
	{
		Sleep(1000);
	}

	sockets::Socket::Cleanup();
	Log(LOG_INFO, __FUNCTION__ " stopped");

	return 0;
}
