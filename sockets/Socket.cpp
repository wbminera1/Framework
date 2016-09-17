#include "Socket.h"
#include "../common/Log.h"

namespace sockets
{

	void Socket::Startup()
	{
		WSADATA wsaData;
		int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (res != 0) {
			Log(LOG_ERR, "WSAStartup failed: %d\n", res);
		}
	}

	void Socket::Cleanup()
	{
		int res = WSACleanup();
		if (res != 0) {
			Log(LOG_ERR, "WSACleanup failed: %d\n", res);
		}
	}

} // namespace socket


