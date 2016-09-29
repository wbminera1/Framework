#include "../common/Log.h"

#include "SocketServer.h"
#include "SocketClient.h"

class TestCommandProcessor : public Dispatched
{
public:
	

	TestCommandProcessor(CommandDispatcher* dispatcher)
		: Dispatched(dispatcher)
	{

	}

	virtual ~TestCommandProcessor()
	{

	}

	virtual bool Receive(Command& cmd)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s", ToString(cmd.m_Command));
		return false;
	}

	virtual bool Send(const Command& cmd)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s", ToString(cmd.m_Command));
		return false;
	}

};


int Server_Test()
{
	Log(LOG_INFO, __FUNCTION__ " started");
	sockets::Socket::Startup();

	CommandDispatcher dispatcher;
	TestCommandProcessor processor(&dispatcher);

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
