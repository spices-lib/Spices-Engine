/**
* @file Socket.h.
* @brief The Socket Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "InetAddress.h"

namespace Spices {

namespace Net {
	
	/**
	* @brief This class is Wrapper of socket.
	*/
	class Socket
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		Socket() = default;

		/**
		* @brief Constructor Function.
		* @param[in] socketFd Socket fd.
		*/
		explicit Socket(SOCKET socketFd)
			: m_SocketFd(socketFd)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Socket();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		Socket(const Socket&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		Socket& operator=(const Socket&) = delete;

		/**
		* @brief Create Non Blocking Socket.
		*/
		void Create();

		/**
		* @brief Get this socket fd.
		* @return Returns this socket fd.
		*/
		const SOCKET Fd() const { return m_SocketFd; }

		/**
		* @brief Get this socket fd.
		* @return Returns this socket fd.
		*/
		SOCKET Fd() { return m_SocketFd; }

		/**
		* @brief Bind address to socket.
		* @param localAddress The address to bind.
		*/
		void BindAddress(const InetAddress& localAddress) const;

		/**
		* @brief Listen on socket.
		*/
		void Listen() const;

		/**
		* @brief Connect to socket
		* @param connectAddress Connection address.
		*/
		void Connect(InetAddress* connectAddress);

		/**
		* @brief Accept connection on socket.
		* @param peerAddress The address of peer.
		* @return Returns the socket fd of peer.
		*/
		SOCKET Accept(InetAddress* peerAddress) const;

		/**
		* @brief Send data to server.
		* @param[in] data Send data.
		*/
		void Send(const std::string& data);

		/**
		* @brief Reveive data from server.
		* @return Returns received data.
		*/
		std::string Receive();

		/**
		* @brief Disable writen in socket.
		*/
		void ShutDownWrite() const;

		/**
		* @brief Set socket Tcp no delay option.
		* @param on True if want enable such option.
		*/
		void SetTcpNoDelay(bool on) const;

		/**
		* @brief Set socket reuse address option.
		* @param on True if want enable such option.
		*/
		void SetReuseAddress(bool on) const;

		/**
		* @brief Set socket reuse port option.
		* @param on True if want enable such option.
		* @attention This option is only available on Linux.
		* use SetReuseAddress instead
		*/
		void SetReusePort(bool on) const;

		/**
		* @brief Set socket keep alive option.
		* @param on True if want enable such option.
		*/
		void SetKeepAlive(bool on) const;

	private:

		/**
		* @brief This socket fd.
		*/
		SOCKET m_SocketFd;
	};

}

}