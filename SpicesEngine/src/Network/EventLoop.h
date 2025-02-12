/**
* @file EventLoop.h.
* @brief The EventLoop Class Definitions.
* @author Spices & Muduo.
*/

#pragma once

#include "Core/Core.h"
#include "Socket.h"
#include <atomic>
#include <windows.h>

namespace Spices {

namespace Net {

	/**
	* @brief Forward Declare.
	*/
	class Channel;
	class Poller;

	/**
	* @brief Wrapper of Poller and wakeup socket to acceptor(SubLoop).
	*/
	class EventLoop
	{
	public:

		using Functor = std::function<void()>;
		using ChannelList = std::vector<Channel*>;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] address ListenAddress.
		*/
		EventLoop(InetAddress* address = nullptr);

		/**
		* @brief Destructor Function.
		*/
		~EventLoop();

		/**
		* @brief Copy Constructor Function.
		* @note This Class not allowed copy behaves.
		*/
		EventLoop(const EventLoop&) = delete;

		/**
		* @brief Copy Assignment Operation.
		* @note This Class not allowed copy behaves.
		*/
		EventLoop& operator=(const EventLoop&) = delete;

		/**
		* @brief Start Event Loop.
		*/
		void Loop();

		/**
		* @brief Quit from Event Loop.
		*/
		void Quit();

		/**
		* @brief Push functor to pending functors.
		* @param[in] cb Functor.
		*/
		void RunInLoop(Functor cb);

		/**
		* @brief Execute functor in EventLoop thread.
		* @param[in] cb Functor.
		*/
		void QueueInLoop(Functor cb);

		/**
		* @brief WakeUp a thread, which wait on recv, by sending one byte data.
		*/
		void WakeUp();

		/**
		* @brief Update channel state with poller.
		* @param[in] channel Channel.
		*/
		void UpdateChannel(Channel* channel) const;

		/**
		* @brief Remove channel from poller.
		* @param[in] channel Channel.
		*/
		void RemoveChannel(Channel* channel) const;

		/**
		* @brief Determine if channel is inside poller.
		* @param[in] channel Channel.
		* @return Returns true if channel is inside poller.
		*/
		bool HasChannel(Channel* channel) const;

		/**
		* @brief Determine if current thread is in eventloop thread.
		* @return Returns true if is in eventloop thread.
		*/
		bool IsInLoopThread() const { return m_ThreadId == GetCurrentThreadId(); }

	private:

		/**
		* @brief Handle WakeUp notify by read one byte data.
		*/
		void HandleWakeUp();

		/**
		* @brief Execute all pending functors.
		*/
		void DoPendingFunctors();

	private:

		/**
		* @brief True if is in Looping.
		*/
		std::atomic_bool m_IsLooping;

		/**
		* @brief True if is quit from Looping.
		*/
		std::atomic_bool m_IsQuit;

		/**
		* @brief True if is execute pending functors.
		*/
		std::atomic_bool m_IsCallingPendingFunctors;

		/**
		* @brief Thread's identify, which is running this EventLoop.
		*/
		DWORD m_ThreadId;

		/**
		* @brief Poller instance.
		*/
		std::shared_ptr<Poller> m_Poller;

		/**
		* @brief Wakeup Socket.
		*/
		Socket m_WakeupFd;

		/**
		* @brief Wakeup Channel.
		*/
		std::unique_ptr<Channel> m_WeakupChannel;

		/**
		* @brief Channels with events.
		*/
		ChannelList m_ActiveChannels;

		/**
		* @brief Delay functors.
		*/
		std::vector<Functor> m_PendingFunctors;

		/**
		* @brief Mutex for PendingFunctors.
		*/
		std::mutex m_Mutex;
	};

	/**
	* @brief Wrapper of Instance/Delete ThreadCache in thread.
	*/
	class EventLoopThreadWrapper
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		EventLoopThreadWrapper() : instance(nullptr) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~EventLoopThreadWrapper() = default;

		/**
		* @brief Get EventLoop Instance.
		* @reutrn Returns EventLoop Instance.
		*/
		static EventLoop*& GetInst(InetAddress* address = nullptr);

	private:

		/**
		* @brief This thread EventLoop instance.
		*/
		EventLoop* instance;
	};

}

}