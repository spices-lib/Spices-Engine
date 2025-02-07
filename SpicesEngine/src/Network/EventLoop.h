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

	class EventLoop
	{
	public:

		using Functor = std::function<void()>;
		using ChannelList = std::vector<Channel*>;

	public:

		/**
		* @brief Constructor Function.
		*/
		EventLoop();

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

		void RunInLoop(Functor cb);

		void QueueInLoop(Functor cb);

		/**
		* @brief WakeUp a thread, which wait on recv, by sending one byte data.
		*/
		void WakeUp();

		void UpdateChannel(Channel* channel);
		void RemoveChannel(Channel* channel);
		bool HasChannel(Channel* channel);

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

		std::atomic_bool m_IsCallingpendingFunctors;

		/**
		* @brief Thread's identify, which is runing this EventLoop.
		*/
		DWORD m_ThreadId;

		std::unique_ptr<Poller> m_Poller;

		Socket m_WakeupFd;
		std::unique_ptr<Channel> m_WeakupChannel;

		ChannelList m_ActiveChannels;
		Channel* m_CurrentActiveChannel;

		std::vector<Functor> m_PendingFunctors;
		std::mutex m_Mutex;
	};

	/**
	* @brief Wapper of Instance/Delete ThreadCache in thread.
	*/
	class EventLoopThreadWapper
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		EventLoopThreadWapper() : instance(nullptr) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~EventLoopThreadWapper() = default;

		/**
		* @brief Get EventLoop Instance.
		* @reutrn Returns EventLoop Instance.
		*/
		EventLoop*& GetInst();

	private:

		/**
		* @brief This thread EventLoop instance.
		*/
		EventLoop* instance;
	};

	/**
	* @brief Thread Unique EventLoop.
	*/
	static _declspec(thread) EventLoopThreadWapper pTLSEventLoop;

}

}