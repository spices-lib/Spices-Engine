/**
* @file Poller.h.
* @brief The Poller Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Core.h"
#include "../InetAddress.h"

namespace Spices {

namespace Net {

	/**
	* @brief Forward Declare.
	*/
	class Channel;
	class EventLoop;

	/**
	* @brief Inherit from this and Implementate Specific Poller
	*/
	class Poller
	{
	public:

		using ChannelList = std::vector<Channel*>;
		using ChannelMap  = std::unordered_map<SOCKET, Channel*>;

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

		/**
		* @brief Poll events on EventList.
		* @param[in] timeoutMs .
		* @param[in,out] activeChannels Channels that events happened.
		*/
		virtual void Poll(int timeoutMs, ChannelList* activeChannels) = 0;

		/**
		* @brief Update Channel and Update Poll.
		* @param[in] channel Channel.
		*/
		virtual void UpdateChannel(Channel* channel) = 0;

		/**
		* @brief Remove Channel and Update Poll.
		* @param[in] channel Channel.
		*/
		virtual void RemoveChannel(Channel* channel) = 0;

		/**
		* @brief Determine if channel is in ChannelMap.
		* @param[in] channel Channel.
		* @return Returns true if channel is in ChannelMap.
		*/
		bool HasChannel(Channel* channel) const;

		/**
		* @brief Create Default Poller.
		* @param[in] loop EventLoop.
		* @return Returns Default Poller.
		*/
		static std::shared_ptr<Poller> DefaultPoller(EventLoop* loop);

	protected:

		/**
		* @brief This Poller interested Channels.
		*/
		ChannelMap m_Channels;

	private:

		/**
		* @brief This Poller interested EventLoop.
		*/
		EventLoop* m_Loop;

	};

}

}