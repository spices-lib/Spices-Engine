/**
* @file DelayThreadPool.h
* @brief The DelayThreadPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "ThreadPoolBasic.h"

namespace Spices {

	/**
	* @brief Wrappers of Delay Thread Pool.
	*/
	class DelayThreadPool : public ThreadPool_Basic<>
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		DelayThreadPool() : ThreadPool_Basic<>(), m_IsStoped(true) {}

		/**
		* @brief Destructor Function.
		*/
		virtual ~DelayThreadPool() override = default;

		/**
		* @brief Continue ThreadPool.
		*/
		void Continue();

		/**
		* @brief Suspend ThreadPool.
		*/
		void Suspend();

		/******************************************Must Implementation************************************************/

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/**
		* @brief Submit a task to task queue, and wait for a idle thread to execute it.
		* @tparam Func Task Function.
		* @tparam Args Task Function Parameter.
		* @return Returns task function return value as a future.
		*/
		template<typename Func, typename... Args>
		auto SubmitPoolTask(Func&& func, Args&&... args) -> std::future<decltype(func(std::forward<Args>(args)...))>;

		/**
		* @brief Thread Function.
		* @param[in] thread Thread Entity.
		*/
		void ThreadFunc(Thread<>* thread);

		/*************************************************************************************************************/

	private:

		/**
		* @brief True if needs stopping execute the task.
		*/
		bool m_IsStoped;
	};

	template<typename Func, typename ...Args>
	inline auto DelayThreadPool::SubmitPoolTask(Func&& func, Args && ...args) -> std::future<decltype(func(std::forward<Args>(args)...))>
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
						auto ptr = std::make_unique<Thread<>>(std::bind(&DelayThreadPool::ThreadFunc, this, std::placeholders::_1), i);
						uint32_t threadId = ptr->GetId();
						
						ptr->Start();
						m_Threads.emplace(threadId, std::move(ptr));

						++m_IdleThreadSize;
						++m_NThreads;

						std::stringstream ss;
						ss << "GameT" << threadId;
						const std::string name = ss.str();
						m_Threads[threadId]->ReceiveThreadTask([=]() {
							ThreadLibrary::SetThreadName(name);
						});
						
						break;
					}
				}
			}
		}

		return result;
	}
}