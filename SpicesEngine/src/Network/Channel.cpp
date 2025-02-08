/**
* @file Channel.cpp.
* @brief The Channel Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "Channel.h"
#include "EventLoop.h"

namespace Spices {

namespace Net {

	Channel::Channel(SOCKET fd, EventLoop* loop)
		: m_Fd(fd)
		, m_Loop(loop)
		, m_Events(EventBits::None)
		, m_Revents(0)
		, m_Index(-1)
	{}

	void Channel::HandleEvent()
	{
		if (m_Tie.has_value())
		{
			if (m_Tie.value().lock())
			{
				HandleEventsWithGuard();
			}
		}
		else
		{
			HandleEventsWithGuard();
		}
	}

	void Channel::Tie(const std::shared_ptr<void>& obj)
	{
		m_Tie = obj;
	}

	void Channel::Remove()
	{
		m_Loop->RemoveChannel(this);
	}

	void Channel::Update()
	{
		m_Loop->UpdateChannel(this);
	}
 
	void Channel::HandleEventsWithGuard() const
	{
		// Close event.
		if ((m_Revents & EPOLLHUP) && !(m_Revents & EPOLLIN))
		{
			if (m_CloseCallback)
			{
				m_CloseCallback();
			}
		}

		// Error event.
		if (m_Revents & EPOLLERR)
		{
			if (m_ErrorCallback)
			{
				m_ErrorCallback();
			}
		}

		// Read event.
		if (m_Revents & (EPOLLIN | EPOLLPRI))
		{
			if (m_ReadCallback)
			{
				m_ReadCallback();
			}
		}

		// Write event.
		if (m_Revents & EPOLLOUT)
		{
			if (m_WriteCallback)
			{
				m_WriteCallback();
			}
		}
	}

}

}