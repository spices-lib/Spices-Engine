#include "Pchheader.h"
#include "HttpServer.h"
#include "HttpRequest.h"

namespace Spices {

namespace Net {

	HttpServer::HttpServer(const InetAddress& listenAddress, int idleSeconds, TcpServer::Option option)
		: m_Server(listenAddress, option)
		, m_HttpCallback(nullptr)
		, m_IdleSeconds(idleSeconds)
	{
		SPICES_PROFILE_ZONE;

		m_Server.AddConnectionCallback(std::bind(&HttpServer::OnConnection, this, std::placeholders::_1));
		m_Server.AddMessageCallback(std::bind(&HttpServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));

		
	}

	void HttpServer::Start(int threadSize) const
	{
		SPICES_PROFILE_ZONE;

		m_Server.Start(threadSize);
	}

	void HttpServer::OnConnection(const TcpConnectionPtr& connection)
	{
		SPICES_PROFILE_ZONE;

		if (connection->Connected())
		{
			SPICES_CORE_INFO("New Http Connection")

			m_ConnectionList.push_back(connection);

			Node node = --m_ConnectionList.end();
			m_NameNodeMap[connection->GetName()] = node;
		}
		else
		{
			SPICES_CORE_INFO("Http Connection Closed")

			assert(m_NameNodeMap.count(connection->GetName()));
			const Node& node = m_NameNodeMap[connection->GetName()];
			m_NameNodeMap.erase(connection->GetName());
			m_ConnectionList.erase(node);
		}
	}

	void HttpServer::OnMessage(const TcpConnectionPtr& connection, Buffer* buf)
	{
		SPICES_PROFILE_ZONE;


	}

	void HttpServer::OnRequest(const HttpRequest& request, HttpResponse* response)
	{
		SPICES_PROFILE_ZONE;
	}

}

}