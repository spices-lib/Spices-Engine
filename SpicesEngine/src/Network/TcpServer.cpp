#include "Pchheader.h"
#include "TcpServer.h"
#include "TcpConnection.h"

namespace Spices {

namespace Net {

	TcpServer::TcpServer(
		EventLoop*         loop          ,
		const InetAddress& listenAddress , 
		Option             option
	)
		: m_Loop(loop)
		, m_NextConnectedId(1)
		, m_Started(0)
	{
		assert(m_Loop);
		m_IpPort = listenAddress.ToIPPort();
		m_Acceptor = std::make_unique<Acceptor>(loop, listenAddress, option == Option::ReusePort);

		m_Acceptor->SetConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
	}

	TcpServer::~TcpServer()
	{
		for (auto& item : m_Connections)
		{
			TcpConnectionPtr connection = item.second;
			item.second.reset();

			connection->GetLoop()->RunInLoop([=]() { connection->ConnectDestroyed(); });
		}
	}

	void TcpServer::Start()
	{
		if (m_Started++ == 0)
		{
			//
			m_Loop->RunInLoop([=]() { m_Acceptor->Listen(); });
		}
	}

	void TcpServer::newConnection(SOCKET socketFd, const InetAddress& peerAddress)
	{
		EventLoop* ioLoop;
		char buf[64] = { 0 };
		snprintf(buf, sizeof(buf), "-%s#%d", m_IpPort.c_str(), m_NextConnectedId);
		++m_NextConnectedId;
		std::string connName = buf;

		sockaddr_in local = {};
		socklen_t addrLen = sizeof(local);
		if (::getsockname(socketFd, (sockaddr*)&local, &addrLen) < 0)
		{
			SPICES_CORE_ERROR("sockets::getsockname")
		}
		InetAddress localAddress(local);

		TcpConnectionPtr connectionPtr = std::make_shared<TcpConnection>(ioLoop, socketFd, localAddress, peerAddress);
		m_Connections[connName] = connectionPtr;

		//
	}

	void TcpServer::removeConnection(const TcpConnectionPtr& connection)
	{
	}
	void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& connection)
	{
	}
}

}