#ifndef SOCKETADDRESS_H_
#define SOCKETADDRESS_H_
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "../common/Statics.h"

namespace sockets
{
	
struct SocketAddress
{
	int m_af;
	size_t m_addrSize;
				
	union
	{
		sockaddr_in  ip4;
		sockaddr_in6 ip6;
	} m_addr;
	
	SocketAddress(int af) : m_af(af) 
	{  
		memset(&m_addr, 0, sizeof(m_addr));
		if (m_af == AF_INET) {
			m_addrSize = sizeof(m_addr.ip4);
			m_addr.ip4.sin_family = AF_INET;
		}
		else if (m_af == AF_INET6) {
			//!! more params
			m_addrSize = sizeof(m_addr.ip6);
			m_addr.ip6.sin6_family = AF_INET6;
		}
		else {
			m_addrSize = 0;
		}
	}

	const SOCKADDR* getAddr() const 
	{
		return (SOCKADDR*)&m_addr;
	}

	const size_t getAddrSize() const
	{
		if (m_af == AF_INET) {
			return sizeof(m_addr.ip4);
		}
		else if (m_af == AF_INET6) {
			return sizeof(m_addr.ip6);
		}
		return 0;
	}


	bool pton(const char* addr)
	{
		int ret = 0;
		if (m_af == AF_INET) {
			ret = inet_pton(AF_INET, addr, &m_addr.ip4.sin_addr);
		}
		else if (m_af == AF_INET6) {
			ret = inet_pton(AF_INET6, addr, &m_addr.ip6.sin6_addr);
		}
		else {
		}
		return ret == 1;
	}
	
	bool ntop(std::string& addr)
	{
		const char* ptr;
		char tmpBuff[statics::Max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN)];
		if (m_af == AF_INET) {
			ptr = inet_ntop(AF_INET, &m_addr.ip4.sin_addr, tmpBuff, sizeof(tmpBuff));
		}
		else if (m_af == AF_INET6) {
			ptr = inet_ntop(AF_INET, &m_addr.ip6.sin6_addr, tmpBuff, sizeof(tmpBuff));
		}
		else {
		}
		if (ptr != nullptr) {
			addr = tmpBuff;
			return true;
		}
		return false;
	}

	void port(int pnum)
	{
		if (m_af == AF_INET) {
			m_addr.ip4.sin_port = htons(pnum);
		}
		else if (m_af == AF_INET6) {
			m_addr.ip6.sin6_port = pnum;
		}
		else {
		}
	}
};
}

#endif // SOCKETADDRESS_H_
