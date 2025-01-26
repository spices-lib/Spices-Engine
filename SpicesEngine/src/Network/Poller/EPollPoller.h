#pragma once
#include "Poller.h"

#include <wepoll.h>

namespace Spices {

namespace Net {

	class EPollPoller : public Poller
	{
	public:

		using EventList = std::vector<epoll_event>;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] loop EventLoop.
		*/
		EPollPoller(EventLoop* loop);

		/**
		* @brief Destructor Function.
		*/
		virtual ~EPollPoller() override;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		EPollPoller(const EPollPoller&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		EPollPoller& operator=(const EPollPoller&) = delete;

		virtual void Poll(int timeoutMs, ChannelList* activeChannels) override;
		virtual void UpdateChannel(Channel* channel) override;
		virtual void RemoveChannel(Channel* channel) override;

	private:

		void FillActiveChannels(int numEvents, ChannelList* activeChannels) const;
		void Update(int operation, Channel* channel);

	private:

		static constexpr int kInitEventListSize = 16;

		SOCKET m_EPollFd;
		EventList m_Events;
	};
}

}