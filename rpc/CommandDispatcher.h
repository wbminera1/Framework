#ifndef COMMANDDISPATCHER_H_
#define COMMANDDISPATCHER_H_
#include "Command.h"

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
		: m_Dispatcher(_dispatcher)
	{
		if (m_Dispatcher != nullptr)
		{
			m_Dispatcher->AddHandler(this);
		}
	}

	virtual ~Dispatched()
	{
		if (m_Dispatcher != nullptr)
		{
			m_Dispatcher->DelHandler(this);
		}
	}

protected:
	CommandDispatcher* m_Dispatcher;
};


#endif // COMMANDDISPATCHER_H_

