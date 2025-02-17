/**
* @file HttpServer.h.
* @brief The HttpServer Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"
#include "Network/Net/TcpServer.h"

namespace Spices {

namespace Net {

	class HttpRequest;
	class HttpResponse;

	class HttpServer
	{
	public:

		using HttpCallback = std::function<void(const HttpRequest&, HttpResponse*)>;
		using WeakTcpConnectionPtr = std::weak_ptr<TcpConnection>;
		using WeakConnectionList = std::list<WeakTcpConnectionPtr>;

		using Node = WeakConnectionList::iterator;
		using NameNode = std::unordered_map<std::string, Node>;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] listenAddress Server listen address.
		* @param[in] idleSeconds .
		* @param[in] option Is reuse port.
		*/
		HttpServer(
			const InetAddress& listenAddress , 
			int                idleSeconds   ,
			TcpServer::Option  option = TcpServer::Option::NoReusePort
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~HttpServer() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		HttpServer(const HttpServer&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		HttpServer& operator=(const HttpServer&) = delete;

		/**
		* @brief Start ThreadPool and call listen on acceptor.
		* @param[in] threadSize ThreadPool size.
		*/
		void Start(int threadSize) const;

	private:

		void OnConnection(const TcpConnectionPtr& connection);
		void OnMessage(const TcpConnectionPtr& connection, Buffer* buf);
		void OnRequest(const TcpConnectionPtr& connection, const HttpRequest& request);

	private:

		TcpServer m_Server;
		HttpCallback m_HttpCallback;
		int m_IdleSeconds;
		
		WeakConnectionList m_ConnectionList;
		NameNode m_NameNodeMap;
	};

}

}