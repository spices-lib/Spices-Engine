#include "Pchheader.h"
#include "Acceptor.h"

namespace Spices {

namespace Net {

	Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddress, bool reusePort)
		: m_Loop(loop)
		, m_AcceptChannel(loop, m_AcceptSocket.Fd())
		, m_IsListening(false)
	{
		m_AcceptSocket.Create();

		m_AcceptSocket.SetReuseAddress(true);
		m_AcceptSocket.SetReusePort(true);
		m_AcceptSocket.BindAddress(listenAddress);
		m_AcceptChannel.SetReadCallback([=]() {HandleRead(); });
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