/**
* @file TcpConnection.h.
* @brief The TcpConnection Class Definitions.
* @author Spices & Muduo.
*/

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

	/**
	* @brief Combine of Socket Connection data.
	*/
	class TcpConnection : public std::enable_shared_from_this<TcpConnection>
	{
	public:

		enum class State 
		{ 
			Disconnected  = 0, 
			Connecting    = 1, 
			Connected     = 2, 
			Disconnecting = 3 
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] ioLoop IO EventLoop.
		* @param[in] name TcpConnection identify.
		* @param[in] socketFd SOCKET.
		* @param[in] localAddress InetAddress.
		* @param[in] peerAddress InetAddress.
		*/
		TcpConnection(
			EventLoop*         ioLoop       ,
			const std::string& name         ,
			SOCKET             socketFd     ,
			const InetAddress& localAddress ,
			const InetAddress& peerAddress
		);

		/**
		* @brief Destructor Function.
		*/
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

		/**
		* @brief Get this TcpConnection IO EventLoop.
		* @return Returns this TcpConnection IO EventLoop.
		*/
		EventLoop* GetLoop() const { return m_IoLoop; }

		/**
		* @brief Get this TcpConnection name.
		* @return Returns this TcpConnection name.
		*/
		const std::string& GetName() const { return m_Name; }

		/**
		* @brief Get this TcpConnection LocalAddress.
		* @return Returns this TcpConnection LocalAddress.
		*/
		const InetAddress& LocalAddress() const { return m_LocalAddress; }

		/**
		* @brief Get this TcpConnection PeerAddress.
		* @return Returns this TcpConnection PeerAddress.
		*/
		const InetAddress& PeerAddress() const { return m_PeerAddress; }

		/**
		* @brief Determined if this TcpConnection is connected.
		* @return Returns true if this TcpConnection is connected.
		*/
		bool Connected() const { return m_State == State::Connected; }

		/**
		* @brief Send message to socket.
		* @param[in] buffer Data.
		*/
		void Send(const std::string& buffer);

		/**
		* @brief ShutDown socket.
		*/
		void ShutDown();

		/**
		* @brief Set ConnectionCallback.
		* @param[in] cb DelegateConnectionCallback.
		*/
		void SetConnectionCallback(const DelegateConnectionCallback& cb) 
		{ 
			m_ConnectionCallback = cb; 
		}

		/**
		* @brief Set MessageCallback.
		* @param[in] cb DelegateMessageCallback.
		*/
		void SetMessageCallback(const DelegateMessageCallback& cb) 
		{ 
			m_MessageCallback = cb; 
		}

		/**
		* @brief Set WriteCompleteCallback.
		* @param[in] cb DelegateWriteCompleteCallback.
		*/
		void SetWriteCompleteCallback(const DelegateWriteCompleteCallback& cb) 
		{ 
			m_WriteCompleteCallback = cb; 
		}

		/**
		* @brief Set HighWaterMarkCallback.
		* @param[in] cb DelegateHighWaterMarkCallback.
		*/
		void SetHighWaterMarkCallback(const DelegateHighWaterMarkCallback& cb) 
		{ 
			m_HighWaterMarkCallback = cb; 
		}

		/**
		* @brief Set CloseCallback.
		* @param[in] cb DelegateCloseCallback.
		*/
		void SetCloseCallback(const DelegateCloseCallback& cb) 
		{ 
			m_CloseCallback = cb; 
		}

		void ConnectEstablished();
		void ConnectDestroyed();

	private:

		void SetState(State state) { m_State = state; }

		/**
		* @brief Handle Read event.
		*/
		void HandleRead();

		/**
		* @brief Handle Write event.
		*/
		void HandleWrite();

		/**
		* @brief Handle Close event.
		*/
		void HandleClose();

		/**
		* @brief Handle Error event.
		*/
		void HandleError() const;

		/**
		* @brief Send message to socket.
		* @param[in] message Data pointer.
		* @param[in] len message bytes.
		*/
		void SendInLoop(const char* message, size_t len);

		/**
		* @brief ShutDown socket.
		*/
		void ShutDownInLoop() const;

	private:

		/**
		* @brief io Loop from TcpServer::NewConnection.
		*/
		EventLoop* m_IoLoop;

		/**
		* @brief TcpConnection name.
		*/
		std::string m_Name;

		/**
		* @brief TcpConnection state.
		*/
		std::atomic<State> m_State;

		/**
		* @brief TcpConnection socket.
		*/
		std::unique_ptr<Socket> m_Socket;

		/**
		* @brief TcpConnection channel.
		*/
		std::unique_ptr<Channel> m_Channel;

		/**
		* @brief TcpConnection LocalAddress.
		*/
		InetAddress m_LocalAddress;

		/**
		* @brief TcpConnection PeerAddress.
		*/
		InetAddress m_PeerAddress;

		/**
		* @brief DelegateConnectionCallback.
		*/
		DelegateConnectionCallback m_ConnectionCallback;

		/**
		* @brief DelegateMessageCallback.
		*/
		DelegateMessageCallback m_MessageCallback;

		/**
		* @brief DelegateWriteCompleteCallback.
		*/
		DelegateWriteCompleteCallback m_WriteCompleteCallback;

		/**
		* @brief DelegateHighWaterMarkCallback.
		*/
		DelegateHighWaterMarkCallback m_HighWaterMarkCallback;

		/**
		* @brief DelegateCloseCallback.
		*/
		DelegateCloseCallback m_CloseCallback;

		/**
		* @brief Input Buffer.
		*/
		Buffer m_InputBuffer;

		/**
		* @brief Output Buffer.
		*/
		Buffer m_OutputBuffer;
	};

}

}