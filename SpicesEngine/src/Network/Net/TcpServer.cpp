/**
* @file TcpServer.cpp.
* @brief The TcpServer Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "EventLoopThreadPool.h"

namespace Spices {

namespace Net {

	TcpServer::TcpServer(
		const InetAddress& listenAddress , 
		Option             option
	)
		: m_NextConnectedId(1)
	{
		SPICES_PROFILE_ZONE;

		m_IpPort = listenAddress.ToIPPort();

		m_Acceptor = std::make_unique<Acceptor>(listenAddress, option == Option::ReusePort);
		m_Acceptor->SetConnectionCallback(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1, std::placeholders::_2));

		m_ThreadPool = std::make_shared<EventLoopThreadPool>(listenAddress);
		m_ThreadPool->SetMode(PoolMode::MODE_FIXED);
	}

	TcpServer::~TcpServer()
	{
		SPICES_PROFILE_ZONE;

		for (auto& item : m_Connections)
		{
			const TcpConnectionPtr connection = item.second;
			item.second.reset();

			EventLoopThreadWrapper::GetInst()->RunInLoop([=]() { connection->ConnectDestroyed(); });
		}
	}

	void TcpServer::Start(int threadSize) const
	{
		SPICES_PROFILE_ZONE;

		if(!m_ThreadPool->IsPoolRunning())
		{
			EventLoopThreadWrapper::GetInst()->RunInLoop([=]() { m_Acceptor->Listen(); });
			m_ThreadPool->Start(threadSize, m_ThreadInitCallback);
		}
	}

	void TcpServer::NewConnection(SOCKET socketFd, const InetAddress& peerAddress)
	{
		SPICES_PROFILE_ZONE;

		EventLoop* ioLoop = m_ThreadPool->GetNextLoop();
		char buf[64] = {};
		snprintf(buf, sizeof(buf), "-%s#%d", m_IpPort.c_str(), m_NextConnectedId);
		++m_NextConnectedId;
		std::string connName = buf;

		sockaddr_in local = {};
		socklen_t addrLen = sizeof(local);
		if (::getsockname(socketFd, (sockaddr*)&local, &addrLen) < 0)
		{
			std::stringstream ss;
			ss << "TcpServer::NewConnection::getsockname error, fd: " << socketFd << ", Error: " << WSAGetLastError();

			SPICES_CORE_ERROR(ss.str())
		}
		InetAddress localAddress(local);

		const TcpConnectionPtr connectionPtr = std::make_shared<TcpConnection>(ioLoop, connName, socketFd, localAddress, peerAddress);
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
		EventLoopThreadWrapper::GetInst()->RunInLoop([=]() { RemoveConnectionInLoop(connection); });
	}

	void TcpServer::RemoveConnectionInLoop(const TcpConnectionPtr& connection)
	{
		SPICES_CORE_INFO("TcpServer::RemoveConnectionInLoop")

		m_Connections.erase(connection->GetName());
		EventLoop* ioLoop = connection->GetLoop();
		ioLoop->QueueInLoop([=]() { connection->ConnectDestroyed(); });
	}
}

}