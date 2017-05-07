#ifndef SOCKETCLIENTBASE_H_
#define SOCKETCLIENTBASE_H_

#include "../sockets/Socket.h"
#include "../thread/Thread.h"

class SocketClientBase : public thread::Thread
{
public:
	SocketClientBase() : Thread(__FUNCTION__) { }
	virtual ~SocketClientBase() {}

	void SetConnection(sockets::Socket& sock, const sockets::SocketAddress& addr);
	void SetAddr(const sockets::SocketAddress& addr);

	sockets::Socket& GetSocket() { return m_Socket; }

	virtual void Stop();

protected:

	virtual void OnStart();
	virtual bool OnIdle();
	virtual void Process();

	sockets::Socket m_Socket;
	sockets::SocketAddress m_Addr;

};


#endif // SOCKETCLIENTBASE_H_