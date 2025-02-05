#pragma once
#include "Core/Core.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include "Core/Thread/ThreadPoolBasic.h"
#include "Callbacks.h"
#include "Buffer.h"
#include "TcpConnection.h"

namespace Spices {

namespace Net {

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

		TcpServer(EventLoop* loop, const InetAddress& listenAddress, Option option = Option::NoReusePort);
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

		void SetThreadInitCallback(const ThreadInitCallback& cb) { m_ThreadInitCallback = cb; }
		void SetConnectionCallback(const DelegateConnectionCallback::Agent& cb) { m_ConnectionCallback.Bind(cb); }
		void SetMessageCallback(const DelegateMessageCallback::Agent& cb) { m_MessageCallback.Bind(cb); }
		void SetWriteCompleteCallback(const DelegateWriteCompleteCallback::Agent& cb) { m_WriteCompleteCallback.Bind(cb); }

		void Start();

	private:

		void newConnection(SOCKET socketFd, const InetAddress& peerAddress);
		void removeConnection(const TcpConnectionPtr& connection);
		void removeConnectionInLoop(const TcpConnectionPtr& connection);

	private:

		EventLoop* m_Loop;
		std::string m_IpPort;

		std::unique_ptr<Acceptor> m_Acceptor;

		std::shared_ptr<ThreadPool> m_ThreadPool;

		DelegateConnectionCallback m_ConnectionCallback;
		DelegateMessageCallback m_MessageCallback;
		DelegateWriteCompleteCallback m_WriteCompleteCallback;

		ThreadInitCallback m_ThreadInitCallback;

		std::atomic_int m_Started;

		int m_NextConnectedId;
		ConnectionMap m_Connections;

	};

}

}