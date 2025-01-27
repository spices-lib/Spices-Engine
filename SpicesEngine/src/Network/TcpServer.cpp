#include "Pchheader.h"
#include "TcpServer.h"

namespace Spices {

namespace Net {
	TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddress, Option option)
	{
	}

	TcpServer::~TcpServer()
	{
		for (auto& item : m_Connections)
		{
			TcpConnectionPtr connection = item.second;
			item.second.reset();


		}
	}

	void TcpServer::Start()
	{
	}
	void TcpServer::newConnection(SOCKET socketFd, const InetAddress& peerAddress)
	{
	}
	void TcpServer::removeConnection(const TcpConnectionPtr& connection)
	{
	}
	void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& connection)
	{
	}
}

}