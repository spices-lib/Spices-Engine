#include "Pchheader.h"
#include "TcpConnection.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"

namespace Spices {

namespace Net {
	TcpConnection::TcpConnection(EventLoop* loop, SOCKET socketFd, const InetAddress& localAddress, const InetAddress& peerAddress)
	{
	}
	TcpConnection::~TcpConnection()
	{
	}
}

}