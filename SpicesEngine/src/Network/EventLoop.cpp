/**
* @file EventLoop.cpp.
* @brief The EventLoop Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Poller/Poller.h"

namespace Spices {

namespace Net {

	/**
	* @brief Poller timeout.
	* Default is 10s.
	*/
	constexpr uint32_t pollTimeoutMs = 10 * 1000;

	EventLoop::EventLoop()
		: m_IsLooping(false)
		, m_IsQuit(false)
		, m_IsCallingPendingFunctors(false)
		, m_CurrentActiveChannel(nullptr)
	{
		SPICES_PROFILE_ZONE;

		m_ThreadId = GetCurrentThreadId();
		m_Poller = Poller::DefaultPoller(this);
		m_WakeupFd.Create();
		m_WeakupChannel = std::make_unique<Channel>(m_WakeupFd.Fd(), this);

		m_WeakupChannel->SetReadCallback([=]() { HandleWakeUp(); });
		m_WeakupChannel->EnableReading();
	}

	EventLoop::~EventLoop()
	{
		m_WeakupChannel->DisableAll();
		m_WeakupChannel->Remove();
	}

	void EventLoop::Loop()
	{
		m_IsLooping = true;
		m_IsQuit = false;

		while (!m_IsQuit)
		{
			m_ActiveChannels.clear();
			m_Poller->Poll(pollTimeoutMs, &m_ActiveChannels);

			for (Channel* channel : m_ActiveChannels)
			{
				channel->HandleEvent();
			}

			DoPendingFunctors();
		}

		m_IsLooping = false;
	}

	void EventLoop::Quit()
	{
		m_IsQuit = true;

		if (!IsInLoopThread())
		{
			WakeUp();
		}
	}

	void EventLoop::RunInLoop(Functor cb)
	{
		if (IsInLoopThread())
		{
			cb();
		}
		else
		{
			QueueInLoop(cb);
		}
	}

	void EventLoop::QueueInLoop(Functor cb)
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_PendingFunctors.emplace_back(cb);
		}

		if (!IsInLoopThread() || m_IsCallingPendingFunctors)
		{
			WakeUp();
		}
	}

	void EventLoop::WakeUp()
	{
		char one = 1;
		int n = ::send(m_WakeupFd.Fd(), &one, sizeof(one), 0);
		if (n < 0)
		{
			std::stringstream ss;
			ss << "EventLoop::WakeUp error, Error: " << WSAGetLastError();

			SPICES_CORE_ERROR(ss.str())
		}
	}

	void EventLoop::UpdateChannel(Channel* channel)
	{
		m_Poller->UpdateChannel(channel);
	}

	void EventLoop::RemoveChannel(Channel* channel)
	{
		m_Poller->RemoveChannel(channel);
	}

	bool EventLoop::HasChannel(Channel* channel)
	{
		return m_Poller->HasChannel(channel);
	}

	void EventLoop::HandleWakeUp()
	{
		char one = 1;
		int n = ::recv(m_WakeupFd.Fd(), &one, sizeof(one), 0);
		if (n < 0)
		{
			std::stringstream ss;
			ss << "EventLoop::HandleWakeUp error, Error: " << WSAGetLastError();

			SPICES_CORE_ERROR(ss.str())
		}
	}

	void EventLoop::DoPendingFunctors()
	{
		std::vector<Functor> functors;
		m_IsCallingPendingFunctors = true;

		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			functors.swap(m_PendingFunctors);
		}

		for (const Functor& functor : functors)
		{
			functor();
		}

		m_IsCallingPendingFunctors = false;
	}

	EventLoop*& EventLoopThreadWrapper::GetInst()
	{
		SPICES_PROFILE_ZONE;

		if (!instance)
		{
			instance = new EventLoop();
		}

		return instance;
	}

}

}