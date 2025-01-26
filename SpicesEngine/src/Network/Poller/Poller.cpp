#include "Pchheader.h"
#include "Poller.h"
#include "Network/Channel.h"

namespace Spices {

namespace Net {

	bool Poller::HasChannel(Channel* channel) const
	{
		SPICES_PROFILE_ZONE;

		auto it = m_Channels.find(channel->Fd());

		return it != m_Channels.end() && it->second == channel;
	}

	Poller* Poller::newDefaultPoller(EventLoop* loop)
	{
		return nullptr;
	}

}

}