/**
* @file EventLoopThreadPool.cpp.
* @brief The EventLoopThreadPool Class Implementation.
* @author Spices & Muduo.
*/

#include "Pchheader.h"
#include "EventLoopThreadPool.h"

namespace Spices {

namespace Net {

	EventLoop* EventLoopThreadPool::GetNextLoop()
	{
		SPICES_PROFILE_ZONE;

		EventLoop* loop = pTLSEventLoop.GetInst();

		if (!m_Loops.empty())
		{
			loop = m_Loops[m_Next];
			++m_Next;

			if (m_Next >= m_Loops.size())
			{
				m_Next = 0;
			}
		}

		return loop;
	}

	std::vector<EventLoop*> EventLoopThreadPool::GetAllLoops()
	{
		SPICES_PROFILE_ZONE;

		if (m_Loops.empty())
		{
			return std::vector<EventLoop*>(1, pTLSEventLoop.GetInst());
		}
		else
		{
			return m_Loops;
		}
	}

	void EventLoopThreadPool::ThreadFunc(Thread<>* thread)
	{
		SPICES_PROFILE_ZONE;

		EventLoop* loop = pTLSEventLoop.GetInst();

		if (m_ThreadInitCallback)
		{
			m_ThreadInitCallback(loop);
		}
		
		m_Loops[thread->GetId()] = loop;

		loop->Loop();
	}

	void EventLoopThreadPool::Start(int initThreadSize, ThreadInitCallback cb)
	{
		SPICES_PROFILE_ZONE;

		m_ThreadInitCallback = std::move(cb);
		m_Loops.resize(initThreadSize);

		m_IsPoolRunning  = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;
		m_NThreads       = initThreadSize;
		
		for (uint32_t i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread<>>(std::bind(&EventLoopThreadPool::ThreadFunc, this, std::placeholders::_1), i);
			int threadId = ptr->GetId();

			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}

	}

}

}