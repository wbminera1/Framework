#include "Socket.h"
#include "SocketAddress.h"
#include "../common/Log.h"


//int main(_In_ int _Argc, _In_reads_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
int Socket_Test()
{
	const char addStrSrc[] = "127.0.0.1";
	sockets::SocketAddress a4(AF_INET);
	sockets::SocketAddress m4(a4);
	a4.pton(addStrSrc);
	std::string addrStrConv;
	a4.ntop(addrStrConv);
	a4.port(9999);
	Log(LOG_INFO, "Addr %s -> %s", addStrSrc, addrStrConv.c_str());

	sockets::Socket::Startup();
	{
		sockets::Socket server;

		server.Open();
		server.Bind(a4);
		server.Listen(4);

		sockets::Socket serverM;

		serverM = std::move(server);

		sockets::Socket in;
		in.Open();

		sockets::SocketAddress c4(AF_INET);
		std::vector<char> data;
		if (in.Accept(serverM, c4)) {
			in.RecV(data);
		}
	}

	sockets::Socket::Cleanup();
	return 0;
}
