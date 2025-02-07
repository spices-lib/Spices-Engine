/**
* @file Acceptor.cpp.
* @brief The Acceptor Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "Acceptor.h"

namespace Spices {

namespace Net {

	Acceptor::Acceptor(const InetAddress& listenAddress, bool reusePort)
		: m_IsListening(false)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create Server Socket.
		*/
		m_AcceptSocket.Create();
		m_AcceptSocket.SetReuseAddress(true);
		m_AcceptSocket.BindAddress(listenAddress);

		m_AcceptChannel = std::make_shared<Channel>(m_AcceptSocket.Fd());
		m_AcceptChannel->SetReadCallback([=]() { HandleRead(); });
	}

	Acceptor::~Acceptor()
	{
		SPICES_PROFILE_ZONE;

		m_AcceptChannel->DisableAll();
		m_AcceptChannel->Remove();
	}

	void Acceptor::Listen()
	{
		SPICES_PROFILE_ZONE;

		m_IsListening = true;
		m_AcceptSocket.Listen();
		m_AcceptChannel->EnableReading();
	}

	void Acceptor::HandleRead()
	{
		InetAddress peerAddress;
		Spices::Net::Socket connect(m_AcceptSocket.Accept(&peerAddress));
		
		if (m_ConnectionCallback)
		{
			m_ConnectionCallback(connect.Fd(), peerAddress);
		}
	}

}

}