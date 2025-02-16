/**
* @file Acceptor.h.
* @brief The Acceptor Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Socket.h"
#include "Channel.h"

namespace Spices {

namespace Net {

	/**
	* @brief Forward Declare.
	*/
	class EventLoop;

	/**
	* @brief Wrapper of Channel and Socket, As the entrance of comm.
	*/
	class Acceptor
	{
	public:

		using ConnectionCallback = std::function<void(SOCKET, const InetAddress&)>;
		  
	public:

		/**
		* @brief Constructor Function.
		* @param[in] listenAddress Acceptor Socket bind address.
		* @param[in] reusePort True if want reuse port.
		*/
		Acceptor(const InetAddress& listenAddress, bool reusePort);

		/**
		* @brief Destructor Function.
		*/
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

		/**
		* @brief Set ConnectionCallback.
		* @param[in] cb ConnectionCallback.
		*/
		void SetConnectionCallback(const ConnectionCallback& cb)
		{
			m_ConnectionCallback = cb;
		}

		/**
		* @brief Determine if this is in listening.
		* @reutrn Returns true if is in listening.
		*/
		bool IsListening() const { return m_IsListening; }

		/**
		* @brief Listen accept socket.
		*/
		void Listen();

	private:

		/**
		* @brief On Read Event Callback.
		*/
		void HandleRead() const;

	private:

		/**
		* @brief Acceptor Socket.
		*/
		Socket m_AcceptSocket;

		/**
		* @brief Acceptor Channel.
		*/
		std::shared_ptr<Channel> m_AcceptChannel;

		/**
		* @brief ConnectionCallback.
		*/
		ConnectionCallback m_ConnectionCallback;

		/**
		* @brief Boolean of whether is in listening.
		*/
		bool m_IsListening;

	};

}

}