#ifndef COMMANDSENDBLOCKING_H_
#define COMMANDSENDBLOCKING_H_
#include "CommandDispatcher.h"

class CommandSendBlocking : public ICommandHandler
{
	pthread_mutex_t count_mutex;
	pthread_cond_t count_threshold_cv;
	volatile bool received;
	eCommand command;

	static const long timeOut = 10; // Timeout in seconds

	virtual bool Handle(Command& cmd);

public:

	Command*  m_Response;

	CommandSendBlocking();
	virtual ~CommandSendBlocking() {}

	bool SendBlocking(const Command& cmd, ICommandHandler& dest, CommandDispatcher& disp);
};

#endif // COMMANDSENDBLOCKING_H_
