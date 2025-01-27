#pragma once
#include "Core/Core.h"
#include "Core/Delegate/DelegateBasic.h"

namespace Spices {

namespace Net {

	class Buffer;
	class TcpConnection;

	using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

	DELEGATE_ONE_PARAM(ConnectionCallback, const TcpConnectionPtr&)
	DELEGATE_ONE_PARAM(CloseCallback, const TcpConnectionPtr&)
	DELEGATE_ONE_PARAM(WriteCompleteCallback, const TcpConnectionPtr&)
	DELEGATE_TWO_PARAM(MessageCallback, const TcpConnectionPtr&, Buffer*)
	DELEGATE_TWO_PARAM(HighWaterMarkCallback, const TcpConnectionPtr&, size_t)

}

}