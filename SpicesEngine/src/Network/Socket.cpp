/**
* @file Socket.h.
* @brief The Socket Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "Socket.h"

namespace Spices {

namespace Net {

	Socket::~Socket()
	{
		SPICES_PROFILE_ZONE;

		::closesocket(m_SocketFd);
	}

	void Socket::CreateNonBlocking()
	{
		SPICES_PROFILE_ZONE;

		SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd < 0)
		{
			std::stringstream ss;
			ss << "Listen socket create error: " << errno;

			SPICES_CORE_CRITICAL(ss.str())
		}

		m_SocketFd = sockfd;
	}

	void Socket::BindAddress(const InetAddress& localAddress) const
	{
		SPICES_PROFILE_ZONE;

		if (::bind(m_SocketFd, (sockaddr*)localAddress.GetSockAddress(), sizeof(sockaddr_in)) < 0)
		{
			std::stringstream ss;
			ss << "Socket::BindAddress failed, socket fd: " << m_SocketFd;

			SPICES_CORE_CRITICAL(ss.str())
		}
	}

	void Socket::Listen() const
	{
		SPICES_PROFILE_ZONE;

		if (::listen(m_SocketFd, 1024) < 0)
		{
			std::stringstream ss;
			ss << "Socket::Listen failed, socket fd: " << m_SocketFd;

			SPICES_CORE_CRITICAL(ss.str())
		}
	}

	void Socket::Connect(InetAddress* connectAddress)
	{
		SPICES_PROFILE_ZONE;

		if (::connect(m_SocketFd, reinterpret_cast<sockaddr*>(connectAddress->GetSockAddress()), sizeof(sockaddr_in)) < 0)
		{
			std::stringstream ss;
			ss << "Socket::Connect failed, socket fd: " << m_SocketFd;

			SPICES_CORE_CRITICAL(ss.str())
		}
	}

	SOCKET Socket::Accept(InetAddress* peerAddress) const
	{
		SPICES_PROFILE_ZONE;

		sockaddr_in address = {};
		socklen_t len = sizeof(address);
		const SOCKET connectFd = ::accept(m_SocketFd, reinterpret_cast<sockaddr*>(&address), &len);

		// Set socket non-blocking.
		u_long mode = 1;
		if(ioctlsocket(m_SocketFd, FIONBIO, &mode) == SOCKET_ERROR)
		{
			std::stringstream ss;
			ss << "Socket::Accept failed, socket fd: " << m_SocketFd;

			SPICES_CORE_CRITICAL(ss.str())
		}
		
		peerAddress->SetSockAddress(address);

		return connectFd;
	}

	void Socket::Send(const std::string& data)
	{
		SPICES_PROFILE_ZONE;

		if (::send(m_SocketFd, data.c_str(), data.size(), 0) < 0)
		{
			std::stringstream ss;
			ss << "Socket::Send error, socket fd: " << m_SocketFd;

			SPICES_CORE_CRITICAL(ss.str())
		}
	}

	std::string Socket::Receive()
	{
		SPICES_PROFILE_ZONE;

		char buffer[1024];
		if (::recv(m_SocketFd, buffer, sizeof(buffer), 0) < 0)
		{
			std::stringstream ss;
			ss << "Socket::Receive error, socket fd: " << m_SocketFd << " Error: " << WSAGetLastError();

			SPICES_CORE_CRITICAL(ss.str())
		}

		return buffer;
	}

	void Socket::ShutDownWrite() const
	{
		SPICES_PROFILE_ZONE;
		
		if (::shutdown(m_SocketFd, SD_SEND) < 0)
		{
			std::stringstream ss;
			ss << "Socket::ShutDownWrite error, socket fd: " << m_SocketFd;

			SPICES_CORE_CRITICAL(ss.str())
		}
	}

	void Socket::SetTcpNoDelay(bool on) const
	{
		SPICES_PROFILE_ZONE;

		::setsockopt(m_SocketFd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&on), sizeof(bool));
	}

	void Socket::SetReusePort(bool on) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @note windows not support such option.
		* Use SetKeepAlive instead.
		*/
		//::setsockopt(m_SocketFd, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<const char*>(&on), sizeof(bool));
		
		SPICES_CORE_ERROR("Windows not support SO_REUSEPORT, use SetReuseAddress instead.")
	}

	void Socket::SetKeepAlive(bool on) const
	{
		SPICES_PROFILE_ZONE;

		::setsockopt(m_SocketFd, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<const char*>(&on), sizeof(bool));
	}

	void Socket::SetReuseAddress(bool on) const
	{
		SPICES_PROFILE_ZONE;

		::setsockopt(m_SocketFd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&on), sizeof(bool));
	}

}

}