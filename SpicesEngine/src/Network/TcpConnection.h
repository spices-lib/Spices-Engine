#pragma once
#include "Core/Core.h"
#include "InetAddress.h"

namespace Spices {

namespace Net {

	class Channel;
	class EventLoop;
	class Socket;

	class TcpConnection : public std::enable_shared_from_this<TcpConnection>
	{
	public:

		TcpConnection(
			EventLoop* loop,
			SOCKET socketFd,
			const InetAddress& localAddress,
			const InetAddress& peerAddress
		);

		virtual ~TcpConnection();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		TcpConnection(const TcpConnection&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		TcpConnection& operator=(const TcpConnection&) = delete;

	private:

	private:

		EventLoop* m_Loop;
		std::atomic_int m_State;
		bool m_Reading;

		std::unique_ptr<Socket> m_Socket;
		std::unique_ptr<Channel> m_Channel;

		InetAddress m_LocalAddress;
		InetAddress m_PeerAddress;


	};

}

}