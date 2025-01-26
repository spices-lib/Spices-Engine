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
		/*: m_IsLooping(false)
		, m_IsQuit(false)
		, m_IsCallingpendingFunctors(false)
		, m_ThreadId(CurrentThread::tid())
		, m_Poller(Poller::newDefaultPoller(this))
		, m_Wakeupfd(createEventfd())
		, m_WeakupChannel(new Channel(this, m_Wakeupfd))
		, m_CurrentActiveChannel(nullptr)*/
	{
		SPICES_PROFILE_ZONE;

		if (pTLSEventLoop)
		{
			SPICES_CORE_CRITICAL("EventLoop is TLS ");
		}
		else
		{
			pTLSEventLoop = this;
		}

		//m_WeakupChannel->SetReadCallback(std::bind(&EventLoop::hanleRead, this));
		//m_WeakupChannel->enableReading();
	}

	EventLoop::~EventLoop()
	{
	}

}

}