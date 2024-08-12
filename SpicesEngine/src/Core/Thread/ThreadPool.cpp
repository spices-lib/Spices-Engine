/**
* @file ThreadPool.h
* @brief The ThreadPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ThreadPool.h"

namespace Spices {

	void ThreadPool::Start(int initThreadSize)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;
		m_NThreads = initThreadSize;

		for (uint32_t i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread<>>(std::bind(&ThreadPool::ThreadFunc, this, std::placeholders::_1), i);
			int threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}
	}

	void ThreadPool::ThreadFunc(Thread<>* thread)
	{
		SPICES_PROFILE_ZONE;

		auto lastTime = std::chrono::high_resolution_clock().now();

		for(;;)
		{
			Task task;
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while (m_Tasks.load() == 0 && thread->GetThreadTasksCount() == 0)
				{
					/**
					* @brief Exit.
					*/
					if (!m_IsPoolRunning)
					{
						m_Threads.erase(thread->GetId());
						--m_IdleThreadSize;
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
								m_Threads.erase(thread->GetId());
								--m_IdleThreadSize;
								--m_NThreads;

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
				* @brief Rink First, Thread Tasks.
				*/
				task = thread->RequireTask();

				/**
				* @brief Rink Second, Pool Tasks.
				*/
				if (!task)
				{
					task = m_TaskQueue.front();
					m_TaskQueue.pop();
					thread->SetThreadInTask(true);
					--m_Tasks;
				}

				--m_IdleThreadSize;
			}

			if (m_Tasks > 0)
			{
				m_NotEmpty.notify_all();
			}

			/**
			* @brief execute task.
			*/
			if (task != nullptr)
			{
				task();
				thread->SetThreadInTask(false);
				++m_IdleThreadSize;
			}
			else
			{
				thread->SetThreadInTask(false);
				++m_IdleThreadSize;
			}
			
			lastTime = std::chrono::high_resolution_clock().now();
		}
	}
}