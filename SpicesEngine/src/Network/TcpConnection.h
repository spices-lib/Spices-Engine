#pragma once
#include "Core/Core.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "Callbacks.h"

namespace Spices {

namespace Net {

	class Channel;
	class EventLoop;
	class Socket;

	class TcpConnection : public std::enable_shared_from_this<TcpConnection>
	{
	public:

		enum class State 
		{ 
			Disconnected = 0, 
			Connecting = 1, 
			Connected = 2, 
			Disconnecting = 3 
		};

	public:

		TcpConnection(
			EventLoop* loop,
			SOCKET socketFd,
			const InetAddress& localAddress,
			const InetAddress& peerAddress
		);

		virtual ~TcpConnection();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		TcpConnection(const TcpConnection&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		TcpConnection& operator=(const TcpConnection&) = delete;

		EventLoop* GetLoop() const { return m_Loop; }
		const InetAddress& LocalAddress() const { return m_LocalAddress; }
		const InetAddress& PeerAddress() const { return m_PeerAddress; }

		bool Connected() const { return m_State == State::Connected; }
		void Send(const std::string& buffer);
		void ShutDown();

		void SetConnectionCallback(const DelegateConnectionCallback& cb) { m_ConnectionCallback = cb; }
		void SetMessageCallback(const DelegateMessageCallback& cb) { m_MessageCallback = cb; }
		void SetWriteCompleteCallback(const DelegateWriteCompleteCallback& cb) { m_WriteCompleteCallback = cb; }
		void SetHighWaterMarkCallback(const DelegateHighWaterMarkCallback& cb) { m_HighWaterMarkCallback = cb; }
		void SetCloseCallback(const DelegateCloseCallback& cb) { m_CloseCallback = cb; }

		void ConnectEstablished();
		void ConnectDestroyed();

	private:

		void SetState(State state) { m_State = state; }

		void HandleRead();
		void HandleWrite();
		void HandleClose();
		void HandleError();

		void SendInLoop(const void* message, size_t len);
		void ShutDownInLoop();

	private:

		EventLoop* m_Loop;
		std::atomic<State> m_State;
		bool m_Reading;

		std::unique_ptr<Socket> m_Socket;
		std::unique_ptr<Channel> m_Channel;

		InetAddress m_LocalAddress;
		InetAddress m_PeerAddress;

		DelegateConnectionCallback m_ConnectionCallback;
		DelegateMessageCallback m_MessageCallback;
		DelegateWriteCompleteCallback m_WriteCompleteCallback;
		DelegateHighWaterMarkCallback m_HighWaterMarkCallback;
		DelegateCloseCallback m_CloseCallback;

		size_t m_HighWaterMark;

		Buffer m_InputBuffer;
		Buffer m_OutputBuffer;
	};

}

}