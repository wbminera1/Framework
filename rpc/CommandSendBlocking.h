#ifndef COMMANDSENDBLOCKING_H_
#define COMMANDSENDBLOCKING_H_
#include "CommandDispatcher.h"
#include "../thread/Mutex.h"
#include "../thread/Condition.h"

class CommandSendBlocking : public ICommandHandler
{
	thread::Mutex count_mutex;
	thread::Condition count_threshold_cv;
	volatile bool received;
	eCommand command;

	static const long timeOut = 10; // Timeout in seconds

	virtual bool Handle(Command& cmd, ICommandHandler* source);

public:

	Command*  m_Response;

	CommandSendBlocking();
	virtual ~CommandSendBlocking() {}

	bool SendBlocking(const Command& cmd, ICommandHandler& dest, CommandDispatcher& disp);
};

#endif // COMMANDSENDBLOCKING_H_
