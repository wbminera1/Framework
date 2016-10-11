#ifndef COMMANDDISPATCHER_H_
#define COMMANDDISPATCHER_H_
#include "Command.h"

class ICommandHandler {
public:
	virtual ~ICommandHandler() {}
	virtual bool Handle(const Command& cmd, ICommandHandler* source) = 0;
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

	bool DispatchFrom(const Command& cmd, ICommandHandler* source)
	{
		for(size_t i = 0; i < m_Handlers.size(); ++i) {
			if (m_Handlers[i] != source) {
				if (m_Handlers[i]->Handle(cmd, source)) {
					return true;
				}
			}
		}
		return false;
	}

	bool DispatchTo(const Command& cmd, ICommandHandler* source)
	{
		for (size_t i = 0; i < m_Handlers.size(); ++i) {
			if (m_Handlers[i] == source) {
				if (m_Handlers[i]->Handle(cmd, source)) {
					return true;
				}
				break;
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
	Dispatched(CommandDispatcher* dispatcher)
		: m_Dispatcher(dispatcher)
	{
		if (m_Dispatcher != nullptr) {
			m_Dispatcher->AddHandler(this);
		}
	}

	virtual ~Dispatched()
	{
		if (m_Dispatcher != nullptr) {
			m_Dispatcher->DelHandler(this);
		}
	}

	CommandDispatcher* GetDispatcher()
	{
		return m_Dispatcher;
	}

	void SetDispatcher(CommandDispatcher* dispatcher)
	{
		if (m_Dispatcher != nullptr) {
			m_Dispatcher->DelHandler(this);
		}
		m_Dispatcher = dispatcher;
		if (m_Dispatcher != nullptr) {
			m_Dispatcher->AddHandler(this);
		}
	}

private:

	CommandDispatcher* m_Dispatcher;
};


#endif // COMMANDDISPATCHER_H_

