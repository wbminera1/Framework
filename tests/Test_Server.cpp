//#include <windows.h>
#include <stdio.h>
#include "../common/Log_Assert.h"

#include "../sockets/SocketServerBase.h"
#include "../sockets/SocketClientThreaded.h"

int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	class TestClient : public sockets::SocketClientThreaded<sockets::SocketRecThread, sockets::SocketSendThread>
	{
	public:
		TestClient()
		{
		}

		~TestClient() { }

	protected:
	};


	class TestServer : public SocketServerBase<TestClient>
	{
	public:
		TestServer() : SocketServerBase(4, 7777) { }
		~TestServer() { }

	};

	sockets::Socket::Startup();

	TestServer testServer;
	testServer.Create();

	sockets::SocketAddress addr("127.0.0.1", 7777, AF_INET);
	TestClient testClient;
	testClient.SetAddr(addr);
	testClient.Create();

	testClient.Join();
	testServer.Join();

	sockets::Socket::Cleanup();

	printf("Done\n");
	return 0;
}
