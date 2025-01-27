#include "Pchheader.h"
#include "EPollPoller.h"
#include "../Channel.h"

namespace Spices {

namespace Net {

	const int kNew = -1;
	const int kAdded = 1;
	const int kDeleted = 2;

	EPollPoller::EPollPoller(EventLoop* loop)
		: Poller(loop)
		, m_EPollFd(reinterpret_cast<SOCKET>(epoll_create1(0)))
		, m_Events(kInitEventListSize)
	{}

	EPollPoller::~EPollPoller()
	{
		::closesocket(m_EPollFd);
	}

	void EPollPoller::Poll(int timeoutMs, ChannelList* activeChannels)
	{
		const int numEvents = epoll_wait(reinterpret_cast<void*>(m_EPollFd), m_Events.data(), m_Events.size(), timeoutMs);
		const int saveErrno = errno;

		if (numEvents > 0)
		{
			FillActiveChannels(numEvents, activeChannels);

			if (numEvents == m_Events.size())
			{
				m_Events.resize(m_Events.size() * 2);
			}
		}
		else if (numEvents == 0)
		{
			SPICES_CORE_INFO("EPollPoller::Poll timeout")
		}
		else
		{
			if (saveErrno != EINTR)
			{
				errno = saveErrno;
				SPICES_CORE_ERROR("EPollPoller::Poll error")
			}
		}
	}

	void EPollPoller::UpdateChannel(Channel* channel)
	{
		const int index = channel->Index();

		if (index == kNew || index == kDeleted)
		{
			if (index == kNew)
			{
				SOCKET fd = channel->Fd();
				m_Channels[fd] = channel;
			}

			channel->SetIndex(kAdded);
			Update(EPOLL_CTL_ADD, channel);
		}
		else
		{
			SOCKET fd = channel->Fd();
			if (channel->IsNoneEvent())
			{
				Update(EPOLL_CTL_DEL, channel);
				channel->SetIndex(kDeleted);
			}
			else
			{
				Update(EPOLL_CTL_MOD, channel);
			}
		}
	}

	void EPollPoller::RemoveChannel(Channel* channel)
	{
		SOCKET fd = channel->Fd();
		m_Channels.erase(fd);

		int index = channel->Index();
		if (index == kAdded)
		{
			Update(EPOLL_CTL_DEL, channel);
		}
		channel->SetIndex(kNew);
	}

	void EPollPoller::FillActiveChannels(int numEvents, ChannelList* activeChannels) const
	{
		for (int i = 0; i < numEvents; ++i)
		{
			Channel* channel = static_cast<Channel*>(m_Events[i].data.ptr);
			channel->SetRevents(m_Events[i].events);
			activeChannels->push_back(channel);
		}
	}

	void EPollPoller::Update(int operation, Channel* channel)
	{
		epoll_event event = {};

		SOCKET fd = channel->Fd();

		event.events = channel->Events();
		event.data.fd = fd;
		event.data.ptr = channel;

		if(::epoll_ctl(reinterpret_cast<void*>(m_EPollFd), operation, fd, &event) < 0)
		{
			if(operation == EPOLL_CTL_DEL)
			{
				SPICES_CORE_ERROR("EPOLL_CTL_DEL error")
			}
			else
			{
				SPICES_CORE_CRITICAL("EPOLL_CTL_ADD/MOD error")
			}
		}
	}

}

}