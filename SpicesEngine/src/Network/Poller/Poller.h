#pragma once
#include "Core/Core.h"
#include "../InetAddress.h"

namespace Spices {

namespace Net {

	class Channel;
	class EventLoop;

	class Poller
	{
	public:

		using ChannelList = std::vector<Channel*>;
		using ChannelMap = std::unordered_map<SOCKET, Channel*>;

	public:

		/**
	    * @brief Constructor Function.
	    * @param[in] loop EventLoop.
	    */
		Poller(EventLoop* loop) : m_Loop(loop) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Poller() = default;

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		Poller(const Poller&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		Poller& operator=(const Poller&) = delete;

		virtual void Poll(int timeoutMs, ChannelList* activeChannels) = 0;
		virtual void UpdateChannel(Channel* channel) = 0;
		virtual void RemoveChannel(Channel* channel) = 0;

		/**
		* @brief 
		*/
		bool HasChannel(Channel* channel) const;

		static Poller* newDefaultPoller(EventLoop* loop);

	protected:

		ChannelMap m_Channels;

	private:

		EventLoop* m_Loop;

	};

}

}