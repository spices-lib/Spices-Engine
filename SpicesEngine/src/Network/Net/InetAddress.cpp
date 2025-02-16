/**
* @file InetAddress.h.
* @brief The InetAddress Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "InetAddress.h"

#include <ws2tcpip.h>

namespace Spices {

namespace Net {

	InetAddress::InetAddress(uint16_t port, std::string ip)
	{
		SPICES_PROFILE_ZONE;

		memset(&m_Address, 0, sizeof(m_Address));

		m_Address.sin_family           = AF_INET;
		m_Address.sin_port             = htons(port);
		m_Address.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
	}

	std::string InetAddress::ToIP() const
	{
		SPICES_PROFILE_ZONE;

		char buf[64] = { 0 };
		::inet_ntop(AF_INET, &m_Address.sin_addr, buf, sizeof(buf));
		return buf;
	}

	std::string InetAddress::ToIPPort() const
	{
		SPICES_PROFILE_ZONE;

		char buf[64] = { 0 };
		::inet_ntop(AF_INET, &m_Address.sin_addr, buf, sizeof(buf));
		const size_t end = strlen(buf);
		const uint16_t port = ntohs(m_Address.sin_port);
		sprintf(buf + end, ":%u", port);
		return buf;
	}

	uint16_t InetAddress::ToPort() const
	{
		SPICES_PROFILE_ZONE;

		return ntohs(m_Address.sin_port);
	}
}

}