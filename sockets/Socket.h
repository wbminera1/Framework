#ifndef SOCKET_H_
#define SOCKET_H_
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "../common/Log.h"
#include "SocketAddress.h"

namespace sockets
{
	
	class Socket
	{
	public:
		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		Socket()
		: m_Socket(INVALID_SOCKET)
		{
		}

		Socket(Socket&& src)
		{
			m_Socket = src.m_Socket;
			src.m_Socket = INVALID_SOCKET;
		}
		
		Socket& operator=(Socket&& src)
		{
			if (this != &src)
			{
				Close();
				m_Socket = src.m_Socket;
				src.m_Socket = INVALID_SOCKET;
			}
			return *this;
		}

		~Socket()
		{
			Close();
		}
		
		bool IsValid() const {
			return m_Socket != INVALID_SOCKET;
		}

		bool Open(int af = AF_INET, int sock = SOCK_STREAM, int ip = IPPROTO_TCP) {
			if (IsValid()) {
				Close();
			}
			m_Socket = socket(af, sock, ip);
			if (!IsValid()) {
				Log(LOG_ERR, "socket function failed with error: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}

		void Close() {
			if (IsValid()) {
				int res = closesocket(m_Socket);
				if (res == SOCKET_ERROR) {
					Log(LOG_ERR, "closesocket function failed with error: %d\n", WSAGetLastError());
				}
				m_Socket = INVALID_SOCKET;
			}
		}

		bool Connect(const SocketAddress& addr)
		{
			auto res = connect(m_Socket, addr.getAddr(), (int)addr.getAddrSize());
			if (res == SOCKET_ERROR) {
				Log(LOG_ERR, "connect function failed with error: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}

		bool Bind(const SocketAddress& addr)
		{
			if (bind(m_Socket, addr.getAddr(), (int)addr.getAddrSize()) == SOCKET_ERROR) {
				Log(LOG_ERR, "bind failed with error: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}

		bool Listen(int maxConnections)
		{
			if (listen(m_Socket, maxConnections) == -1)
			{
				Log(LOG_ERR, "listen failed with error: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}

		bool Accept(const Socket& listened, SocketAddress& connectedAddr)
		{
			if (IsValid()) {
				Close();
			}
			m_Socket = accept(listened.m_Socket, (sockaddr *)&connectedAddr.m_addr, (int*)&connectedAddr.m_addrSize);
			if (!IsValid())
			{
				Log(LOG_ERR, "accept error: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}

		bool RecV(std::vector<char>& data, size_t dataSize = 1024)
		{
			size_t currentSize = data.size();
			if (currentSize < dataSize) {
				data.resize(dataSize);
				const int flags = 0;
				auto bytes_received = recv(m_Socket, &data[currentSize], (int)(data.capacity() - currentSize), flags);
				if (bytes_received > 0) {
					Log(LOG_INFO, "RecV - received %d\n", bytes_received);
					data.resize(bytes_received + currentSize);
				}
				else if (bytes_received == 0) {
					Log(LOG_INFO, "RecV - host shutdown.\n");
				}
				else { // (bytes_received < 0)
					Log(LOG_ERR, "RecV - receive error: %d\n", bytes_received);
				}
				return bytes_received != 0;
			}
			return false;
		}

		int Send(const std::vector<char>& data)
		{
			const int flags = 0;
			auto bytes_sent = send(m_Socket, &data[0], (int)data.size(), flags);
			if (bytes_sent > 0) {
				Log(LOG_INFO, "Send - sent %d\n", bytes_sent);
			}
			else if (bytes_sent == 0) {
				Log(LOG_INFO, "Send - host shutdown.\n");
			}
			else { // (bytes_received < 0)
				Log(LOG_ERR, "Send - receive error: %d\n", WSAGetLastError());
			}
			return bytes_sent;
		}

		static void Startup();
		static void Cleanup();

	protected:
		SOCKET m_Socket;
	};

} // namespace socket


#endif // SOCKET_H_