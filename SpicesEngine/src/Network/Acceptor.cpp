#include "Pchheader.h"
#include "Acceptor.h"

namespace Spices {

namespace Net {

	static SOCKET createNonblocking()
	{
		/*SOCKET sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
		if (sockfd < 0)
		{
			LOG_FATAL("%s:%s:%d listen socket create err:%d \n", __FILE__, __FUNCTION__, __LINE__, errno);
		}*/

		return 0;
	}

	Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddress, bool reusePort)
		: m_Loop(loop)
		, m_AcceptSocket(createNonblocking())
		, m_AcceptChannel(loop, m_AcceptSocket.Fd())
		, m_IsListening(false)
	{
		m_AcceptSocket.SetReuseAddress(true);
		m_AcceptSocket.SetReusePort(true);
		m_AcceptSocket.BindAddress(listenAddress);
		m_AcceptChannel.SetReadCallback(std::bind(&Acceptor::HandleRead, this));
	}

	Acceptor::~Acceptor()
	{
		m_AcceptChannel.DisableAll();
		m_AcceptChannel.Remove();
	}

	void Acceptor::Listen()
	{
		m_IsListening = true;
		m_AcceptSocket.Listen();
		m_AcceptChannel.EnableReading();
	}

	void Acceptor::HandleRead()
	{
		InetAddress peerAddress;
		SOCKET connectFd = m_AcceptSocket.Accept(&peerAddress);
		
		if (m_ConnectionCallback)
		{
			m_ConnectionCallback(connectFd, peerAddress);
		}
		else
		{
			::closesocket(connectFd);
		}
	}

}

}