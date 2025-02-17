/**
* @file HttpServer.cpp.
* @brief The HttpServer Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpContext.h"

namespace Spices {

namespace Net {

	HttpServer::HttpServer(
		const InetAddress& listenAddress , 
		int                idleSeconds   , 
		TcpServer::Option  option
	)
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

		auto context = std::make_unique<HttpContext>();

		assert(m_NameNodeMap.count(connection->GetName()));
		Node node = m_NameNodeMap[connection->GetName()];

		m_ConnectionList.splice(m_ConnectionList.end(), m_ConnectionList, node);
		assert(node == --m_ConnectionList.end());

		if (!context->ParseRequest(buf))
		{
			SPICES_CORE_INFO("HttpContext ParseRequest Failed")

			connection->Send("HTTP/1.1 400 Bad Request\r\n\r\n");
			connection->ShutDown();
		}

		if (context->GotAll())
		{
			SPICES_CORE_INFO("HttpContext ParseRequest Succeed")

			OnRequest(connection, context->GetRequest());
			context->Reset();
		}
	}

	void HttpServer::OnRequest(const TcpConnectionPtr& connection, const HttpRequest& request)
	{
		SPICES_PROFILE_ZONE;

		std::string connectionHeader = request.GetHeader("Connection");

		bool close = connectionHeader == "close" || (request.GetVersion() == HttpRequest::Version::HTTP10 && connectionHeader != "Keep-Alive");

		HttpResponse response(close);
		m_HttpCallback(request, &response);

		Buffer buf;
		response.AppendToBuffer(&buf);
		connection->Send(buf.RetrieveAllAsString());

		if (response.CloseConnection())
		{
			connection->ShutDown();
		}
	}

}

}