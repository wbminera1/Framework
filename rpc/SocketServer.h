#ifndef SOCKETLISTENER_H_
#define SOCKETLISTENER_H_

#include "../thread/Thread.h"
#include "../thread/Mutex.h"

#include "Command.h"

class SocketServer : public Dispatched, public thread::Thread {
public:
	explicit SocketServer(int port, CommandDispatcher* dispatcher);
	virtual ~SocketServer();

	virtual bool Receive(Command& cmd) { return false; }
	virtual bool Send(const Command& cmd) { return false; }

	bool Start();
//	void stop();


private:

	virtual void Process() { Start(); }

	//void loop();

    //static const size_t MAX_COMMAND_SIZE = 4 * 1024;
    //static const size_t MAX_BINARY_SIZE = 2 * 1024 * 1024;

	int m_Port;

	thread::Mutex requestMutex;
	//std::string   commandToSend;

	//volatile bool stopLoop;

	//void Handle(std::string& str, std::vector<char>& binaryData);
};

#endif /* SOCKETLISTENER_H_ */
