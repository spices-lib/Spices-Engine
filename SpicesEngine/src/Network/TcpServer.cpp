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

		m_ThreadPool = std::make_shared<ThreadPool>();
		m_ThreadPool->SetMode(PoolMode::MODE_FIXED);
		m_ThreadPool->Start(4);
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

		TcpConnectionPtr connectionPtr = std::make_shared<TcpConnection>(ioLoop, connName, socketFd, localAddress, peerAddress);
		m_Connections[connName] = connectionPtr;

		connectionPtr->SetConnectionCallback(m_ConnectionCallback);
		connectionPtr->SetMessageCallback(m_MessageCallback);
		connectionPtr->SetWriteCompleteCallback(m_WriteCompleteCallback);

		DelegateCloseCallback closeCallback;
		closeCallback.Bind([=](const TcpConnectionPtr& connection) { removeConnection(connection); });
		connectionPtr->SetCloseCallback(closeCallback);

		ioLoop->RunInLoop([=]() { connectionPtr->ConnectEstablished(); });
	}

	void TcpServer::removeConnection(const TcpConnectionPtr& connection)
	{
		m_Loop->RunInLoop([=]() { removeConnectionInLoop(connection); });
	}

	void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& connection)
	{
		SPICES_CORE_INFO("TcpServer::removeConnectionInLoop")

		m_Connections.erase(connection->GetName());
		EventLoop* ioLoop = connection->GetLoop();
		ioLoop->QueueInLoop([=]() { connection->ConnectDestroyed(); });
	}
}

}