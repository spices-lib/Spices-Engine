/**
* @file VulkanCmdThreadPool.h.
* @brief The VulkanCmdThreadPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Core/Thread/ThreadPoolBasic.h"
#include "VulkanCommandBuffer.h"
#include "Render/FrameInfo.h"

namespace Spices {

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
		virtual ~VulkanCmdThreadPool() override;

		/******************************************Must Implementation************************************************/

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency());

		/**
		* @brief Submit a part commands task to task queue, and wait for a idle thread to execute it.
		* @tparam func Task Function.
		*/
		template<typename RType, typename Func, typename ...Args>
		auto SubmitPoolTask(Func&& func, Args && ...args) -> std::future<RType>;

		/**
		* @brief Thread Function.
		* @param[in] thread Thread Entity.
		*/
		void ThreadFunc(Thread<VkCommandBuffer>* thread);

		/*************************************************************************************************************/

		/**
		* @brief GetCommandPools.
		* @return Return all CommandPools.
		*/
		std::vector<VkCommandPool>& GetCommandPools() { return m_CmdPools; }

		/**
		* @brief GetCommandBuffers.
		* @param[in] FrameIndex in FrameInfo
		* @return Return all CommandBuffers.
		*/
		std::vector<VkCommandBuffer>& GetCommandBuffers(int frameIndex) { return m_CmdBuffers[frameIndex]; }

	private:

		/**
		* @brief Parallel CommandPools.
		*/
		std::vector<VkCommandPool> m_CmdPools;

		/**
		* @brief Parallel Secondary CommandBuffers.
		*/
		std::array<std::vector<VkCommandBuffer>, MaxFrameInFlight> m_CmdBuffers;
	};

	template<typename RType, typename Func, typename ...Args>
	inline auto VulkanCmdThreadPool::SubmitPoolTask(Func&& func, Args && ...args) -> std::future<RType>
	{
		SPICES_PROFILE_ZONE;

		auto task = std::make_shared<std::packaged_task<RType(VkCommandBuffer)>>(std::bind(std::forward<Func>(func), std::placeholders::_1, std::forward<Args>(args)...));
		std::future<RType> result = task->get_future();

		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			/**
			* @brief pack task as a lambda and submit it to queue.
			*/
			m_TaskQueue.emplace([task](VkCommandBuffer cmdBuffer) { (*task)(cmdBuffer); });
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
						auto ptr = std::make_unique<Thread<VkCommandBuffer>>(std::bind(&VulkanCmdThreadPool::ThreadFunc, this, std::placeholders::_1), i);
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
