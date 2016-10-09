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

	typedef bool (TestCommandProcessor::*ReceiveCommandFn)(const Command& cmd);

	virtual bool Handle(const Command& cmd, ICommandHandler* source)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s", ToString(cmd.m_Command));
		/*
		DECL_ENUM(cFirst),
		//
		DECL_ENUM(cError),
		DECL_ENUM(cResponse),
		DECL_ENUM(cConnect),
		DECL_ENUM(cDisconnect),
		DECL_ENUM(cExit),
		DECL_ENUM(cJSONCommand),
		DECL_ENUM(cBinaryCommand),
		//
		DECL_ENUM(cLast)

		*/
		ReceiveCommandFn fpa[cLast] = { nullptr, nullptr, nullptr, &TestCommandProcessor::ReceiveCommandConnect, nullptr, nullptr, nullptr, nullptr };
		if (fpa[cmd.m_Command] != nullptr) {
			return (this->*fpa[cmd.m_Command])(cmd);
		}
		return ReceiveCommand(cmd);
	}

	bool ReceiveCommand(const Command& cmd)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s not handled", ToString(cmd.m_Command));
		return false;
	}

	bool ReceiveCommandConnect(const Command& cmd)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s", ToString(cmd.m_Command));
		const CommandConnect& cmdConnect((const CommandConnect&)cmd);
		Log(LOG_INFO, __FUNCTION__ " Version %d", cmdConnect.m_Version);
		CommandResponse cmdResponse(cmd, cmdConnect.m_Version == Command::VERSION ? sOk : sFail);
		GetDispatcher()->Dispatch(cmdResponse, this);
		return false;
	}

	bool ReceiveCommandDisconnect(const Command& cmd)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s not handled", ToString(cmd.m_Command));
		return false;
	}

	bool ReceiveCommandResponse(const Command& cmd)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s not handled", ToString(cmd.m_Command));
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
