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

	/*SOCKET createEventfd()
	{
		SOCKET evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
		if (evtfd < 0)
		{
			LOG_FATAL("eventfd error:%d \n", errno);
		}

		return evtfd;
	}*/

	EventLoop::EventLoop()
		: m_IsLooping(false)
		, m_IsQuit(false)
		, m_IsCallingpendingFunctors(false)
		, m_ThreadId(GetCurrentThreadId())
		, m_Poller(Poller::newDefaultPoller(this))
		//, m_Wakeupfd(createEventfd())
		//, m_WeakupChannel(new Channel(this, m_Wakeupfd))
		, m_CurrentActiveChannel(nullptr)
	{
		SPICES_PROFILE_ZONE;

		if (pTLSEventLoop)
		{
			SPICES_CORE_CRITICAL("EventLoop is TLS ")
		}
		else
		{
			pTLSEventLoop = this;
		}

		m_WeakupChannel->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
		m_WeakupChannel->EnableReading();
	}

	EventLoop::~EventLoop()
	{
		m_WeakupChannel->DisableAll();
		m_WeakupChannel->Remove();
		::closesocket(m_WakeupFd);
		pTLSEventLoop = nullptr;
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

		if (!IsInLoopThread() || m_IsCallingpendingFunctors)
		{
			WakeUp();
		}
	}

	void EventLoop::WakeUp()
	{
		uint64_t one = 1;
		size_t n = write(m_WakeupFd, &one, sizeof(one));
		if (n != sizeof(one))
		{
			SPICES_CORE_ERROR("EventLoop::HandleRead write error")
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

	void EventLoop::HandleRead()
	{
		uint64_t one = 1;
		size_t n = read(m_WakeupFd, &one, sizeof(one));
		if (n != sizeof(one))
		{
			SPICES_CORE_ERROR("EventLoop::HandleRead read error")
		}
	}

	void EventLoop::DoPendingFunctors()
	{
		std::vector<Functor> functors;
		m_IsCallingpendingFunctors = true;

		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			functors.swap(m_PendingFunctors);
		}

		for (const Functor& functor : functors)
		{
			functor();
		}

		m_IsCallingpendingFunctors = false;
	}

}

}