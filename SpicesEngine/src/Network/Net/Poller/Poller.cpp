/**
* @file Poller.cpp.
* @brief The Poller Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "Poller.h"
#include "Network/Net/Channel.h"
#include "EPollPoller.h"

namespace Spices {

namespace Net {

	bool Poller::HasChannel(Channel* channel) const
	{
		SPICES_PROFILE_ZONE;

		auto it = m_Channels.find(channel->Fd());

		return it != m_Channels.end() && it->second == channel;
	}

	std::shared_ptr<Poller> Poller::DefaultPoller(EventLoop* loop)
	{
		return std::make_shared<EPollPoller>(loop);
	}

}

}