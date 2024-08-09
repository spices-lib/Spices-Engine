/**
* @file VulkanCmdThreadPool.h.
* @brief The VulkanCmdThreadPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Core/Thread/ThreadPool.h"
#include "VulkanCommandBuffer.h"

namespace Spices {

	/**
	* @brief VulkanCmdThreadPool Class.
	* This class defines the VulkanCmdThreadPool behaves.
	* Is use for submit commands parallel.
	*/
	class VulkanCmdThreadPool : public VulkanObject, public ThreadPool_Basic<VkCommandBuffer>
	{
	public:

		/**
		* @brief Constructor Function.
		* Create Specific ThreadPool.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanCmdThreadPool(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanCmdThreadPool() override = default;

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/**
		* @brief GetCommandBuffers.
		* @return Return all CommandBuffers.
		*/
		std::vector<VkCommandBuffer>& GetCommandBuffers() { return m_CmdBuffers; }

		/**
		* @brief Submit a task to task queue, and wait for a idle thread to execute it.
		* @tparam func Task Function.
		*/
		template<typename RType>
		auto SubmitTask(std::function<RType(VkCommandBuffer cmdBuffer)> func) -> std::future<RType>;

	protected:

		/**
		* @brief Thread Function.
		* @param[in] threadid Thread id from std
		*/
		void ThreadFunc(uint32_t threadid);

	private:

		/**
		* @brief Parallel Secondary CommandBuffers.
		*/
		std::vector<VkCommandBuffer> m_CmdBuffers;
	};

	template<typename RType>
	inline auto VulkanCmdThreadPool::SubmitTask(std::function<RType(VkCommandBuffer cmdBuffer)> func) -> std::future<RType>
	{
		SPICES_PROFILE_ZONE;

		auto task = std::make_shared<std::packaged_task<RType(VkCommandBuffer)>>(func);
		std::future<RType> result = task->get_future();

		std::unique_lock<std::mutex> lock(m_Mutex);

		if (!m_NotFull.wait_for(lock, std::chrono::seconds(1), [&]() { return m_TaskQueue.size() < (size_t)TASK_MAX_THRESHHOLD; }))
		{
			SPICES_CORE_WARN("Task Submit failed");
		
			auto emptyTask = std::make_shared<std::packaged_task<RType()>>([]() { return RType(); });
			(*emptyTask)();
			return emptyTask->get_future();
		}

		/**
		* @brief pack task as a lambda and submit it to queue.
		*/
		m_TaskQueue.emplace([task](VkCommandBuffer cmdBuffer) {(*task)(cmdBuffer); });
		m_NotEmpty.notify_all();

		/**
		* @brief Expand threads container if in MODE_CACHED.
		*/
		if (m_PoolMode == PoolMode::MODE_CACHED && m_TaskQueue.size() > m_IdleThreadSize && m_Threads.size() < THREAD_MAX_THRESHHOLD)
		{
			auto ptr = std::make_unique<Thread>(std::bind(&VulkanCmdThreadPool::ThreadFunc, this, std::placeholders::_1));
			ptr->Start();
			uint32_t threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));
		
			++m_IdleThreadSize;
		}

		return result;
	}
}
