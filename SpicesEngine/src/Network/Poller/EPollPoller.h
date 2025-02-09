/**
* @file EPollPoller.h.
* @brief The EPollPoller Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Poller.h"

#include <wepoll.h>

namespace Spices {

namespace Net {

	/**
	* @brief EPoll Poller.
	*/
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

		/**
		* @brief Poll events on EventList.
		* @param[in] timeoutMs .
		* @param[in,out] activeChannels Channels that events happened.
		*/
		virtual void Poll(int timeoutMs, ChannelList* activeChannels) override;

		/**
		* @brief Update Channel and Update Poll.
		* @param[in] channel Channel.
		*/
		virtual void UpdateChannel(Channel* channel) override;

		/**
		* @brief Remove Channel and Update Poll.
		* @param[in] channel Channel.
		*/
		virtual void RemoveChannel(Channel* channel) override;

	private:

		/**
		* @brief Fill REvents to ChannelList.
		* @param[in] numEvents .
		* @param[in] activeChannels ChannelList.
		*/
		void FillActiveChannels(int numEvents, ChannelList* activeChannels) const;

		/**
		* @brief Update epoll with operation.
		* @param[in] operation.
		* @param[in] channel Channel.
		*/
		void Update(int operation, Channel* channel);

	private:

		/**
		* @brief EPoll fd.
		*/
		SOCKET m_EPollFd;

		/**
		* @brief EventList.
		*/
		EventList m_Events;
	};
}

}