/**
* @file InetAddress.h.
* @brief The InetAddress Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"

#include <winsock2.h>
#include<ws2tcpip.h>

namespace Spices {

namespace Net {
	
	/**
	* @brief This class is Wrapper of current socket address.
	* @attention Not support IpV6.
	*/
	class InetAddress
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] port .
		* @param[in] ip .
		*/
		InetAddress(uint16_t port = 0, std::string ip = "127.0.0.1");

		/**
		* @brief Constructor Function.
		* @param[in] addr sockaddr_in.
		*/
		InetAddress(const sockaddr_in& addr)
			: m_Address(addr)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~InetAddress() = default;

		/**
		* @brief Get IP from Socket Address.
		* @return Returns IP.
		*/
		std::string ToIP() const;

		/**
		* @brief Get IP and Port from Socket Address.
		* @return Returns IP and Port.
		*/
		std::string ToIPPort() const;

		/**
		* @brief Get Port and Port from Socket Address.
		* @return Returns Port.
		*/
		uint16_t ToPort() const;

		/**
		* @brief Get Socket Address.
		* @return Returns Socket Address pointer.
		*/
		const sockaddr_in* GetSockAddress() const { return &m_Address; }

		/**
		* @brief Get Socket Address.
		* @return Returns Socket Address pointer.
		*/
		sockaddr_in* GetSockAddress() { return &m_Address; }
		
		/**
		* @brief Set Socket Address.
		* @param[in] addr sockaddr_in.
		*/
		void SetSockAddress(const sockaddr_in& addr) { m_Address = addr; }

	private:

		/**
		* @brief Socket Address.
		*/
		sockaddr_in m_Address;

	};

}

}