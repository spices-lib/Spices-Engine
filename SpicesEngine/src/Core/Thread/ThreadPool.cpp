/**
* @file ThreadPool.h
* @brief The ThreadPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ThreadPool.h"

namespace Spices {

	uint32_t Thread::m_GeneraterId = 0;

	Thread::Thread(ThreadFunc func)
		: m_Func(func)
		, m_ThreadId(m_GeneraterId++)
	{}

	void Thread::Start()
	{
		SPICES_PROFILE_ZONE;

		std::thread t(m_Func, m_ThreadId);
		t.detach();
	}

	void ThreadPool::Start(int initThreadSize)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;

		for (int i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::ThreadFunc, this, std::placeholders::_1));
			int threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}
	}

	void ThreadPool::ThreadFunc(uint32_t threadid)
	{
		SPICES_PROFILE_ZONE;

		auto lastTime = std::chrono::high_resolution_clock().now();

		for(;;)
		{
			Task task;
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while (m_TaskQueue.size() == 0)
				{
					/**
					* @brief Exit.
					*/
					if (!m_IsPoolRunning)
					{
						m_Threads.erase(threadid);
						m_IdleThreadSize--;
						m_ExitCond.notify_all();
						return;
					}

					if (m_PoolMode == PoolMode::MODE_CACHED)
					{
						if (m_NotEmpty.wait_for(lock, std::chrono::seconds(1)) == std::cv_status::timeout)
						{
							auto now = std::chrono::high_resolution_clock().now();
							auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);

							/**
							* @brief Try recovery unused threads.
							*/
							if (dur.count() >= m_ThreadIdleTimeOut && m_Threads.size() > m_InitThreadSize)
							{
								m_Threads.erase(threadid);
								m_IdleThreadSize--;

								return;
							}
						}
					}
					else
					{
						m_NotEmpty.wait(lock);
					}
				}

				/**
				* @brief Get a task from task queue.
				*/
				task = m_TaskQueue.front();
				m_TaskQueue.pop();

				--m_IdleThreadSize;
				m_NotFull.notify_all();
				if (m_TaskQueue.size() > 0) m_NotEmpty.notify_all();
			}

			/**
			* @brief execute task.
			*/
			if (task != nullptr)
			{
				task();
				++m_IdleThreadSize;
			}
			else
			{
				++m_IdleThreadSize;
			}
			
			lastTime = std::chrono::high_resolution_clock().now();
		}
	}
}