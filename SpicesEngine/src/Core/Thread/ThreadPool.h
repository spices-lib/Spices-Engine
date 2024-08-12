/**
* @file ThreadPool.h
* @brief The ThreadPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "ThreadPoolBasic.h"

namespace Spices {

	class ThreadPool : public ThreadPool_Basic<>
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ThreadPool() : ThreadPool_Basic<>() {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadPool() override {};

		/******************************************Must Implementation************************************************/

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/**
		* @brief Submit a task to task queue, and wait for a idle thread to execute it.
		* @tparam Func Task Function.
		* @tparam Args Task Funcion Parameter.
		* @return Returns task function return value as a future.
		*/
		template<typename Func, typename... Args>
		auto SubmitPoolTask(Func&& func, Args&&... args) -> std::future<decltype(func(std::forward<Args>(args)...))>;

		/**
		* @brief Thread Function.
		* @param[in] thread Thread Entity.
		*/
		void ThreadPool::ThreadFunc(Thread<>* thread);

		/*************************************************************************************************************/
	};

	template<typename Func, typename ...Args>
	inline auto ThreadPool::SubmitPoolTask(Func&& func, Args && ...args) -> std::future<decltype(func(std::forward<Args>(args)...))>
	{
		SPICES_PROFILE_ZONE;

		using RType = decltype(func(args...));

		/**
		* @brief pack function pointer with packaged_task and get future.
		*/
		auto task = std::make_shared<std::packaged_task<RType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
		std::future<RType> result = task->get_future();

		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			/**
			* @brief pack task as a lambda and submit it to queue.
			*/
			m_TaskQueue.emplace([task]() {(*task)(); });
			++m_Tasks;

			/**
			* @brief Expand threads container if in MODE_CACHED.
			*/
			if (m_PoolMode == PoolMode::MODE_CACHED && m_TaskQueue.size() > m_IdleThreadSize && m_Threads.size() < THREAD_MAX_THRESHHOLD)
			{
				for (uint32_t i = 0; i < THREAD_MAX_THRESHHOLD; i++)
				{
					if (m_Threads.find(i) == m_Threads.end())
					{
						auto ptr = std::make_unique<Thread<>>(std::bind(&ThreadPool::ThreadFunc, this, std::placeholders::_1), i);
						ptr->Start();
						uint32_t threadId = ptr->GetId();
						m_Threads.emplace(threadId, std::move(ptr));

						++m_IdleThreadSize;
						++m_NThreads;
						break;
					}
				}
			}
		}

		m_NotEmpty.notify_all();

		return result;
	}
}