#include "Pchheader.h"
#include "EPollPoller.h"

namespace Spices {

namespace Net {

	EPollPoller::EPollPoller(EventLoop* loop)
		: Poller(loop)
		//, m_EPollFd(epoll_create1(EPOLL_CLOEXEC))
		, m_Events(kInitEventListSize)
	{}

	EPollPoller::~EPollPoller()
	{
		::closesocket(m_EPollFd);
	}

	void EPollPoller::Poll(int timeoutMs, ChannelList* activeChannels)
	{
		int numEvents = epoll_wait((void*)m_EPollFd, m_Events.data(), m_Events.size(), timeoutMs);
		int saveErrno = errno;

		if (numEvents > 0)
		{
			FillActiveChannels(numEvents, activeChannels);

			if (numEvents == m_Events.size())
			{
				m_Events.resize(m_Events.size() * 2);
			}
		}
	}

	void EPollPoller::UpdateChannel(Channel* channel)
	{
	}

	void EPollPoller::RemoveChannel(Channel* channel)
	{
	}

	void EPollPoller::FillActiveChannels(int numEvents, ChannelList* activeChannels) const
	{
	}

	void EPollPoller::Update(int operation, Channel* channel)
	{
	}

}

}