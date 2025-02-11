/**
* @file TcpServer.h.
* @brief The TcpServer Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "TcpConnection.h"

namespace Spices {

namespace Net {

	/**
	* @brief Forward Declare.
	*/
	class EventLoopThreadPool;

	/**
	* @brief Sample of a TcpServer.
	*/
	class TcpServer
	{
	public:

		using ThreadInitCallback = std::function<void(EventLoop*)>;
		using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

		enum class Option
		{
			NoReusePort = 0,
			ReusePort = 1
		};

		/**
		* @brief Constructor Function.
		* @param[in] listenAddress Server listen address.
		* @param[in] option Is reuse port.
		*/
		TcpServer(const InetAddress& listenAddress, Option option = Option::NoReusePort);

		/**
		* @brief Destructor Function.
		*/
		virtual ~TcpServer();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		TcpServer(const TcpServer&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		TcpServer& operator=(const TcpServer&) = delete;

		/**
		* @brief Set Thread initialize Call back.
		* @param[in] cb ThreadInitCallback.
		*/
		void SetThreadInitCallback(const ThreadInitCallback& cb) 
		{
			m_ThreadInitCallback = cb;
		}

		/**
		* @brief Add Connection Call back.
		* @param[in] cb DelegateConnectionCallback::Agent.
		*/
		void AddConnectionCallback(const DelegateConnectionCallback::Agent& cb) 
		{ 
			m_ConnectionCallback.Bind(cb); 
		}

		/**
		* @brief Add Message Call back.
		* @param[in] cb DelegateMessageCallback::Agent.
		*/
		void AddMessageCallback(const DelegateMessageCallback::Agent& cb) 
		{ 
			m_MessageCallback.Bind(cb); 
		}

		/**
		* @brief Add WriteComplete Call back.
		* @param[in] cb DelegateWriteCompleteCallback::Agent.
		*/
		void AddWriteCompleteCallback(const DelegateWriteCompleteCallback::Agent& cb) 
		{ 
			m_WriteCompleteCallback.Bind(cb);
		}

		/**
		* @brief Start ThreadPool and call listen on acceptor.
		* @param[in] threadSize ThreadPool size.
		*/
		void Start(int threadSize) const;

	private:
		
		/**
		* @brief Handle New Connection to Acceptor.
		* @param socketFd Accept return SOCKET.
		* @param peerAddress Accept return Address.
		*/
		void NewConnection(SOCKET socketFd, const InetAddress& peerAddress);

		/**
		* @brief Remove a TcpConnection.
		* @param connection TcpConnectionPtr.
		*/
		void RemoveConnection(const TcpConnectionPtr& connection);

		/**
		* @brief Remove a TcpConnection InLoop.
		* @param connection TcpConnectionPtr.
		*/
		void RemoveConnectionInLoop(const TcpConnectionPtr& connection);

	private:

		/**
		* @brief TcpServer Ip and Port.
		*/
		std::string m_IpPort;

		/**
		* @brief Acceptor.
		*/
		std::unique_ptr<Acceptor> m_Acceptor;

		/**
		* @brief ThreadPool
		*/
		std::shared_ptr<EventLoopThreadPool> m_ThreadPool;

		/**
		* @brief ConnectionCallback.
		*/
		DelegateConnectionCallback m_ConnectionCallback;

		/**
		* @brief MessageCallback.
		*/
		DelegateMessageCallback m_MessageCallback;

		/**
		* @brief WriteCompleteCallback.
		*/
		DelegateWriteCompleteCallback m_WriteCompleteCallback;

		/**
		* @brief ThreadInitCallback.
		*/
		ThreadInitCallback m_ThreadInitCallback;

		int m_NextConnectedId;
		ConnectionMap m_Connections;

	};

}

}