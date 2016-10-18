#include "../common/Log.h"
#include "../sockets/SocketClientThreaded.h"


class ProxyClient : public sockets::SocketClientThreaded<sockets::SocketRecThread, sockets::SocketSendThread>
{
public:
	ProxyClient()  
	{ 
	}
	
	~ProxyClient() { }

	protected:
};


class ProxyServer : public SocketServerBase<ProxyClient>
{
public:
	ProxyServer() : SocketServerBase(4, 7777) { }
	~ProxyServer() { }

};


int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	Log(LOG_INFO, "Started...");
	sockets::Socket::Startup();
	ProxyServer pserver;
	pserver.Create();
	thread::Thread::Sleep(100);
	pserver.Join();
	sockets::Socket::Cleanup();
	return 0;
}
