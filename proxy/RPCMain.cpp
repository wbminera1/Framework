#include "../common/Log.h"
//#include "../sockets/SocketClientThreaded.h"
#include "../rpc/SocketServerRPC.h"
#include "../rpc/SocketClientRPC.h"

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

	typedef bool (TestCommandProcessor::*ReceiveCommandFn)(const Command& cmd, ICommandHandler* source);

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
			return (this->*fpa[cmd.m_Command])(cmd, source);
		}
		return ReceiveCommand(cmd, source);
	}

	bool ReceiveCommand(const Command& cmd, ICommandHandler* source)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s not handled", ToString(cmd.m_Command));
		return false;
	}

	bool ReceiveCommandConnect(const Command& cmd, ICommandHandler* source)
	{
		Log(LOG_INFO, __FUNCTION__ " Command %s", ToString(cmd.m_Command));
		const CommandConnect& cmdConnect((const CommandConnect&)cmd);
		Log(LOG_INFO, __FUNCTION__ " Version %d", cmdConnect.m_Version);
		CommandResponse cmdResponse(cmd, cmdConnect.m_Version == Command::VERSION ? sOk : sFail);
		GetDispatcher()->DispatchTo(cmdResponse, source);
		return false;
	}

	bool ReceiveCommandDisconnect(const Command& cmd, ICommandHandler* source)
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

int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	Log(LOG_INFO, "Started...");
	sockets::Socket::Startup();
	CommandDispatcher dispatcher;
	TestCommandProcessor processor(&dispatcher);
	SocketServerRPC<SocketClientRPC> pserver(16, 9999, &dispatcher);
	pserver.Create();
	thread::Thread::Sleep(100);
	pserver.Join();
	sockets::Socket::Cleanup();
	return 0;
}
