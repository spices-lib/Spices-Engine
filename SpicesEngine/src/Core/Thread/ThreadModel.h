/**
* @file ThreadModel.h
* @brief The ThreadModel Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "ThreadPoolBasic.h"
#include "Render/Vulkan/VulkanCmdThreadPool.h"
#include "Core/Container/ThreadQueue.h"

namespace Spices {

	/**
	* @brief ThreadPool Type Enum.
	*/
	enum class ThreadPoolEnum
	{
		Main    = 0,    /* @brief Main Thread.       */
		Game    = 1,    /* @brief Game ThreadPool.   */
		RHI     = 2,    /* @brief RHI ThreadPool.    */
		Custom  = 3,    /* @brief Custom ThreadPool. */
	};

	/**
	* @brief Wrappers of all Thread Pool.
	*/
	class ThreadModel
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		ThreadModel();

		/**
		* @brief Destructor Function.
		*/
		virtual ~ThreadModel() = default;

		/**
		* @brief Get ThreadModel single instance.
		* @return Returns this single instance.
		*/
		static std::shared_ptr<ThreadModel> Get();

		/**
		* @brief Init Custom ThreadPool.
		*/
		void InitCustomThreadPool();

		/**
		* @brief Init Game ThreadPool.
		*/
		void InitGameThreadPool();

		/**
		* @brief Init RHI ThreadPool.
		* @param[in] fn RHI ThreadPool Create function pointer.
		*/
		void InitRHIThreadPool(std::function<void(std::shared_ptr<VulkanCmdThreadPool>& ptr)> fn);

		/**
		* @brief Shutdown this ThreadModel.
		*/
		void ShutDownThreadModel();

		/**
		* @brief Get Custom ThreadPool.
		* @return Returns Custom ThreadPool.
		*/
		std::shared_ptr<ThreadPool> GetCustomThreadPool() { return m_CustomThreadPool; }

		/**
		* @brief Get Game ThreadPool.
		* @return Returns Game ThreadPool.
		*/
		std::shared_ptr<ThreadPool> GetGameThreadPool() { return m_GameThreadPool; }

		/**
		* @brief Get RHI ThreadPool.
		* @return Returns RHI ThreadPool.
		*/
		std::shared_ptr<VulkanCmdThreadPool> GetRHIThreadPool() { return m_RHIThreadPool; }

		/**
		* @brief Get MainThread Task Queue.
		* @return Returns MainThread Task Queue.
		*/
		scl::thread_queue<std::function<void()>>& GetMainTaskQueue() { return m_MainThreadTasks; }

	private:

		/**
		* @brief Shutdown Custom ThreadPool.
		*/
		void ShutDownCustomThreadPool();

		/**
		* @brief Shutdown Game ThreadPool.
		*/
		void ShutDownGameThreadPool();

		/**
		* @brief Shutdown RHI ThreadPool.
		*/
		void ShutDownRHIThreadPool();

		/**
		* @brief Clear task queue in main thread.
		*/
		void ClearMainThreadTaskQueue();

	private:

		/**
		* @brief Get ThreadModel single instance.
		*/
		static std::shared_ptr<ThreadModel> m_ThreadModel;

		/**
		* @brief Custom ThreadPool.
		*/
		std::shared_ptr<ThreadPool> m_CustomThreadPool;

		/**
		* @brief Game ThreadPool.
		*/
		std::shared_ptr<ThreadPool> m_GameThreadPool;

		/**
		* @brief RHI ThreadPool.
		*/
		std::shared_ptr<VulkanCmdThreadPool> m_RHIThreadPool;

		/**
		* @brief Tasks must be done in main thread.
		*/
		scl::thread_queue<std::function<void()>> m_MainThreadTasks;
	};
	
	template<typename F, typename ...Args>
	static auto AsyncTask(ThreadPoolEnum pool, F&& func, Args&&... args) -> std::future<decltype(func(std::forward<Args>(args)...))>
	{
		SPICES_PROFILE_ZONE;

		if (!(pool == ThreadPoolEnum::Game || pool == ThreadPoolEnum::Custom))
		{
			SPICES_CORE_ERROR("task shouble be submit to game/custom thread")
			return std::future<decltype(func(std::forward<Args>(args)...))>();
		}

		switch (pool)
		{
		case Spices::ThreadPoolEnum::Game:
			return ThreadModel::Get()->GetGameThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
		case Spices::ThreadPoolEnum::Custom:
			return ThreadModel::Get()->GetCustomThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
		}
	}

	template<typename F, typename ...Args>
	static auto AsyncRHITask(ThreadPoolEnum pool, F&& func, Args&&... args) -> std::future<decltype(func(nullptr, std::forward<Args>(args)...))>
	{
		SPICES_PROFILE_ZONE;

		assert(pool == ThreadPoolEnum::RHI);

		return ThreadModel::Get()->GetRHIThreadPool()->SubmitPoolTask(func, std::forward<Args>(args)...);
	}

	template<typename Func, typename ...Args>
	inline void AsyncMainTask(ThreadPoolEnum pool, Func&& func, Args && ...args)
	{
		SPICES_PROFILE_ZONE;

		assert(pool == ThreadPoolEnum::Main);

		ThreadModel::Get()->GetMainTaskQueue().Push(std::bind(func, std::forward<Args>(args)...));
	}
}