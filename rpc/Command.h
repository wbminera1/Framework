#ifndef COMMAND_H_
#define COMMAND_H_
#include <string>
#include <vector>
#include <cstdint>
#include <pthread.h>

#include "CommandEnums.h"

/**
 *
 */
struct Command
{
	eCommand command;

	Command() : command(cError) {}
	explicit Command(eCommand _command) : command(_command) {}

	void Serialize(std::vector<char>& commandBuffer) const;
	void Deserialize(const std::vector<char>& commandBuffer);

	template <typename T> void toBuffer(std::vector<char>& commandBuffer, const T& field) const
	{
		std::vector<char> tmpBuffer;
		tmpBuffer.assign((char*)&field, (char*)&field + sizeof(field));
		commandBuffer.insert(commandBuffer.end(), tmpBuffer.begin(), tmpBuffer.end());
	}

	static void Test(void);

};

class ICommandHandler {
public:
	virtual ~ICommandHandler() {}
	virtual bool Receive(Command& cmd) = 0;
	virtual bool Send(const Command& cmd) = 0;
};

class CommandDispatcher
{
public:
	CommandDispatcher()
{
		m_Handlers.reserve(16);
}
	virtual ~CommandDispatcher() {}

	void AddHandler(ICommandHandler* handler)
	{
		for (size_t i = 0; i < m_Handlers.size(); ++i) {
			if (m_Handlers[i] == handler) {
				return;
			}
		}
		m_Handlers.push_back(handler);
	}

	void DelHandler(ICommandHandler* handler)
	{
		for(size_t i = 0; i < m_Handlers.size(); ++i) {
			if (m_Handlers[i] == handler) {
				m_Handlers.erase(m_Handlers.begin() + i);
				break;
			}
		}
	}

	bool Dispatch(Command& cmd)
	{
		for(size_t i = 0; i < m_Handlers.size(); ++i) {
			if (m_Handlers[i]->Receive(cmd)) {
				return true;
			}
		}
		return false;
	}

private:
	std::vector<ICommandHandler*> m_Handlers;
};

class Dispatched : public ICommandHandler
{
public:
	Dispatched(CommandDispatcher* _dispatcher)
		: dispatcher(_dispatcher)
	{
		if (dispatcher != nullptr)
		{
			dispatcher->AddHandler(this);
		}
	}

	virtual ~Dispatched()
	{
		if (dispatcher != nullptr)
		{
			dispatcher->DelHandler(this);
		}
	}

protected:
	CommandDispatcher* dispatcher;
};

class CommandSendBlocking : public ICommandHandler
{
	pthread_mutex_t count_mutex;
	pthread_cond_t count_threshold_cv;
	volatile bool received;
	eCommand command;

	static const long timeOut = 10; // Timeout in seconds

	virtual bool Handle(Command& cmd);

public:

	Command*  response;

	CommandSendBlocking();
	virtual ~CommandSendBlocking() {}

	bool SendBlocking(const Command& cmd, ICommandHandler& dest, CommandDispatcher& disp);
};

#endif /* COMMAND_H_ */
