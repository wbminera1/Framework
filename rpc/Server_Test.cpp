#include <stdio.h>
#include "../common/Log.h"
#include "SocketServer.h"
#include "SocketClient.h"
#include "../thread/Thread.h"
//#include "test_accept.h"


int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{

	sockets::Socket::Startup();

	CommandDispatcher dispatcher;

	SocketServer sl(9999, &dispatcher);
	sl.Create();
	Sleep(1000);
/*
	SocketClient sc(19999, &dispatcher);
	sc.Create();
	Command cmd;
	cmd.command = cPing;

	Sleep(1000);

	sc.send(cmd);
	*/

	while (true)
	{
		Sleep(1000);
	}

	sockets::Socket::Cleanup();

	return 0;
}
