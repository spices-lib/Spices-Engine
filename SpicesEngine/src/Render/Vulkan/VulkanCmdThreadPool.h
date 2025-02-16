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
#include "Core/Reflect/StaticReflect/FunctionTraits.h"

namespace Spices {

	/**
	* @brief Wrappers of RHI Thread Pool.
	*/
	class VulkanCmdThreadPool : public VulkanObject, public ThreadPool_Basic<VkCommandBuffer>
	{
	public:

		/**
		* @brief Constructor Function.
		* Create Specific ThreadPool.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] name ThreadPool Name.
		*/
		VulkanCmdThreadPool(VulkanState& vulkanState, const std::string& name);

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
		* @tparam Func Task Function.
		* @tparam Args Task Parameters.
		*/
		template<typename Func, typename ...Args>
		auto SubmitPoolTask(Func&& func, Args && ...args) -> std::future<decltype(func(nullptr, std::forward<Args>(args)...))>;

		/**
		* @brief Thread Function.
		* @param[in] thread Thread Entity.
		*/
		void ThreadFunc(Thread<VkCommandBuffer>* thread);

		/*************************************************************************************************************/

		/**
		* @brief Create second CommandBuffer in rhi threadPool thread.
		* @param[in] threadId .
		* @return Returns second CommandBuffer.
		*/
		VkCommandBuffer CreateParallelCommandBuffers(uint32_t threadId);

		/**
		* @brief Free all second CommandBuffer in rhi threadPool thread.
		* @param[in] frameIndex current frame index.
		*/
		void FreeParallelCommandBuffers(uint32_t frameIndex);

	private:

		/**
		* @brief Parallel CommandPools.
		*/
		std::vector<VkCommandPool> m_CmdPools;

		/**
		* @brief Parallel Secondary CommandBuffers.
		*/
		std::array<std::vector<std::vector<VkCommandBuffer>>, MaxFrameInFlight> m_CmdBuffers;
	};

	template<typename Func, typename ...Args>
	inline auto VulkanCmdThreadPool::SubmitPoolTask(Func&& func, Args && ...args) -> std::future<decltype(func(nullptr, std::forward<Args>(args)...))>
	{
		SPICES_PROFILE_ZONE;

		using RType = decltype(func(nullptr, args...));

		auto task = std::make_shared<std::packaged_task<RType(VkCommandBuffer)>>(std::bind(std::forward<Func>(func), std::placeholders::_1, std::forward<Args>(args)...));
		std::future<decltype(func(nullptr, std::forward<Args>(args)...))> result = task->get_future();
		
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
						uint32_t threadId = ptr->GetId();

						ptr->Start();
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
