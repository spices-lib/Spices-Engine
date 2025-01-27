#pragma once
#include "Socket.h"
#include "Channel.h"

namespace Spices {

namespace Net {

	class EventLoop;

	class Acceptor
	{
	public:

		using ConnectionCallback = std::function<void(SOCKET, const InetAddress&)>;

	public:

		Acceptor(EventLoop* loop, const InetAddress& listenAddress, bool reusePort);
		virtual ~Acceptor();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		Acceptor(const Acceptor&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		Acceptor& operator=(const Acceptor&) = delete;

		void SetConnectionCallback(const ConnectionCallback& cb)
		{
			m_ConnectionCallback = cb;
		}

		bool IsListening() const { return m_IsListening; }
		void Listen();

	private:

		void HandleRead();

	private:

		EventLoop* m_Loop;
		Socket m_AcceptSocket;
		Channel m_AcceptChannel;

		ConnectionCallback m_ConnectionCallback;
		bool m_IsListening;

	};

}

}