#include "Pchheader.h"
#include "TcpServer.h"
#include "TcpConnection.h"

namespace Spices {

namespace Net {

	TcpServer::TcpServer(
		const InetAddress& listenAddress , 
		Option             option
	)
		: m_Started(false)
		, m_NextConnectedId(1)
	{
		SPICES_PROFILE_ZONE;

		m_IpPort = listenAddress.ToIPPort();

		m_Acceptor = std::make_unique<Acceptor>(listenAddress, option == Option::ReusePort);
		m_Acceptor->SetConnectionCallback(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1, std::placeholders::_2));

		m_ThreadPool = std::make_shared<ThreadPool>();
		m_ThreadPool->SetMode(PoolMode::MODE_FIXED);
	}

	TcpServer::~TcpServer()
	{
		SPICES_PROFILE_ZONE;

		for (auto& item : m_Connections)
		{
			TcpConnectionPtr connection = item.second;
			item.second.reset();

			pTLSEventLoop.GetInst()->RunInLoop([=]() { connection->ConnectDestroyed(); });
		}
	}

	void TcpServer::Start()
	{
		SPICES_PROFILE_ZONE;

		if (!m_Started.load())
		{
			m_ThreadPool->Start(4);
			pTLSEventLoop.GetInst()->RunInLoop([=]() { m_Acceptor->Listen(); });
		}
	}

	void TcpServer::NewConnection(SOCKET socketFd, const InetAddress& peerAddress)
	{
		SPICES_PROFILE_ZONE;

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

		TcpConnectionPtr connectionPtr = std::make_shared<TcpConnection>(connName, socketFd, localAddress, peerAddress);
		m_Connections[connName] = connectionPtr;

		connectionPtr->SetConnectionCallback(m_ConnectionCallback);
		connectionPtr->SetMessageCallback(m_MessageCallback);
		connectionPtr->SetWriteCompleteCallback(m_WriteCompleteCallback);

		DelegateCloseCallback closeCallback;
		closeCallback.Bind([=](const TcpConnectionPtr& connection) { RemoveConnection(connection); });
		connectionPtr->SetCloseCallback(closeCallback);

		ioLoop->RunInLoop([=]() { connectionPtr->ConnectEstablished(); });
	}

	void TcpServer::RemoveConnection(const TcpConnectionPtr& connection)
	{
		pTLSEventLoop.GetInst()->RunInLoop([=]() { RemoveConnectionInLoop(connection); });
	}

	void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& connection)
	{
		SPICES_CORE_INFO("TcpServer::removeConnectionInLoop")

		m_Connections.erase(connection->GetName());
		//EventLoop* ioLoop = connection->GetLoop();
		//ioLoop->QueueInLoop([=]() { connection->ConnectDestroyed(); });
	}
}

}