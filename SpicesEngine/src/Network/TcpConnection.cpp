#include "Pchheader.h"
#include "TcpConnection.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

namespace Spices {

namespace Net {

	TcpConnection::TcpConnection(
		const std::string& name,
		SOCKET             socketFd     ,
		const InetAddress& localAddress ,
		const InetAddress& peerAddress
	)
		: m_Name(name)
		, m_State(State::Connecting)
		, m_Reading(true)
		, m_LocalAddress(localAddress)
		, m_PeerAddress(peerAddress)
		, m_HighWaterMark(64 * 1024 * 1024)
	{
		m_Socket = std::make_unique<Socket>(socketFd);
		m_Channel = std::make_unique<Channel>(socketFd);

		m_Channel->SetReadCallback([=]() { HandleRead(); });
		m_Channel->SetWriteCallback([=]() { HandleWrite(); });
		m_Channel->SetCloseCallback([=]() { HandleClose(); });
		m_Channel->SetErrorCallback([=]() { HandleError(); });

		m_Socket->SetKeepAlive(true);
	}

	TcpConnection::~TcpConnection()
	{
	}

	void TcpConnection::Send(const std::string& buffer)
	{
		if (m_State.load() == State::Connected)
		{
			if(pTLSEventLoop.GetInst()->IsInLoopThread())
			{
				SendInLoop(buffer.c_str(), buffer.size());
			}
			else
			{
				pTLSEventLoop.GetInst()->RunInLoop([=]() { SendInLoop(buffer.c_str(), buffer.size()); });
			}
		}
	}

	void TcpConnection::ShutDown()
	{
		if (m_State == State::Connected)
		{
			SetState(State::Disconnecting);

			pTLSEventLoop.GetInst()->RunInLoop([=]() { ShutDownInLoop(); });
		}
	}

	void TcpConnection::ConnectEstablished()
	{
		SetState(State::Connected);
		m_Channel->Tie(shared_from_this());
		m_Channel->EnableReading();

		m_ConnectionCallback.Broadcast(shared_from_this());
	}

	void TcpConnection::ConnectDestroyed()
	{
		if (m_State.load() == State::Connected)
		{
			SetState(State::Disconnected);

			m_Channel->DisableAll();
			m_CloseCallback.Broadcast(shared_from_this());
		}
		m_Channel->Remove();
	}

	void TcpConnection::HandleRead()
	{
		int saveErrno = 0;
		size_t n = m_InputBuffer.ReadFd(m_Channel->Fd(), &saveErrno);
		if (n > 0)
		{
			m_MessageCallback.Broadcast(shared_from_this(), &m_InputBuffer);
		}
		else if (n == 0)
		{
			HandleClose();
		}
		else
		{
			errno = saveErrno;
			SPICES_CORE_ERROR("TcpConnection::HandleRead Error")
			HandleError();
		}
	}

	void TcpConnection::HandleWrite()
	{
		if (m_Channel->IsWriting())
		{
			int saveErrno = 0;
			size_t n = m_OutputBuffer.WriteFd(m_Channel->Fd(), &saveErrno);
			if (n > 0)
			{
				m_OutputBuffer.Retrieve(n);
				if (m_OutputBuffer.ReadableBytes() == 0)
				{
					m_Channel->DisableWriting();
					if (!m_WriteCompleteCallback.empty())
					{
						pTLSEventLoop.GetInst()->QueueInLoop([=]() { m_WriteCompleteCallback.Broadcast(shared_from_this()); });
					}
					if (m_State.load() == State::Disconnecting)
					{
						ShutDownInLoop();
					}
				}
			}
			else
			{
				SPICES_CORE_ERROR("TcpConnection::HandleWrite Error")
			}
		}
		else
		{
			SPICES_CORE_ERROR("TcpConnection::HandleWrite Error")
		}
	}

	void TcpConnection::HandleClose()
	{
		SetState(State::Disconnected);
		m_Channel->DisableAll();

		const TcpConnectionPtr connectionPtr = shared_from_this();
		m_ConnectionCallback.Broadcast(connectionPtr);
		m_CloseCallback.Broadcast(connectionPtr);
	}

	void TcpConnection::HandleError()
	{
		char optVal;
		socklen_t optLen = sizeof(optVal);
		int err = 0;
		if (::getsockopt(m_Channel->Fd(), SOL_SOCKET, SO_ERROR, &optVal, &optLen) < 0)
		{
			err = errno;
		}
		else
		{
			err = optVal;
		}

		std::stringstream ss;
		ss << "TcpConnection::HandleError SO_ERROR: " << err;

		SPICES_CORE_ERROR(ss.str())
	}

	void TcpConnection::SendInLoop(const void* message, size_t len)
	{
		size_t nWrote = 0;
		size_t remaining = len;
		bool faultError = false;

		if (m_State.load() == State::Disconnected)
		{
			SPICES_CORE_ERROR("Disconnected")
			return;
		}

		if (!m_Channel->IsWriting() && m_OutputBuffer.ReadableBytes() == 0)
		{
			nWrote = ::_write(m_Channel->Fd(), message, len);

			remaining = len - nWrote;
			if (remaining == 0 && m_WriteCompleteCallback.size() > 0)
			{
				pTLSEventLoop.GetInst()->QueueInLoop([=]() { m_WriteCompleteCallback.Broadcast(shared_from_this()); });
			}
		}

		if (!faultError && remaining > 0)
		{
			size_t oldLen = m_OutputBuffer.ReadableBytes();
			if (oldLen + remaining >= m_HighWaterMark && oldLen < m_HighWaterMark && m_HighWaterMarkCallback.size() > 0)
			{
				pTLSEventLoop.GetInst()->QueueInLoop([=]() { m_HighWaterMarkCallback.Broadcast(shared_from_this(), oldLen + remaining); });
			}
			m_OutputBuffer.Append((char*)message + nWrote, remaining);
			if (!m_Channel->IsWriting())
			{
				m_Channel->EnableWriting();
			}
		}
	}

	void TcpConnection::ShutDownInLoop()
	{
		if (!m_Channel->IsWriting())
		{
			m_Socket->ShutDownWrite();
		}
	}
}

}