/**
* @file VulkanCmdThreadPool.cpp.
* @brief The VulkanCmdThreadPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanCmdThreadPool.h"
#include "Render/FrameInfo.h"

namespace Spices {

	const int nCmdThreads = 24;

	VulkanCmdThreadPool::VulkanCmdThreadPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
		, ThreadPool_Basic()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Init ThreadPool.
		*/
		{
			SetMode(PoolMode::MODE_FIXED);
			Start(nCmdThreads);
		}

		/**
		* @brief Create Parallel CommandPool.
		*/
		{

			m_CmdPools.resize(nCmdThreads);

			/**
			* @brief Instanced a VkCommandPoolCreateInfo with default value.
			*/
			VkCommandPoolCreateInfo       poolInfo{};
			poolInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags              = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex   = vulkanState.m_GraphicQueueFamily;

			/**
			* @brief Create commandpool and set it global.
			*/
			for (int i = 0; i < nCmdThreads; i++)
			{
				VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &m_CmdPools[i]));
				VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_COMMAND_POOL, m_CmdPools[i], vulkanState.m_Device, "ParallelCommandPool");
			}
		}

		/**
		* @brief Create Parallel CommandBuffer.
		*/
		{
			for (int i = 0; i < MaxFrameInFlight; i++)
			{
				m_CmdBuffers[i].resize(nCmdThreads);
			}

			for (int i = 0; i < nCmdThreads; i++)
			{
				VkCommandBufferAllocateInfo       allocInfo{};
				allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				allocInfo.commandPool           = m_CmdPools[i];
				allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
				allocInfo.commandBufferCount    = 1;

				for (int j = 0; j < MaxFrameInFlight; j++)
				{
					VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, &m_CmdBuffers[j][i]));
					VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_COMMAND_BUFFER, m_CmdBuffers[j][i], vulkanState.m_Device, "ParallelCommandBuffer");
				}
			}
		}
	}

	void VulkanCmdThreadPool::Start(int initThreadSize)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;

		for (int i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread>(std::bind(&VulkanCmdThreadPool::ThreadFunc, this, std::placeholders::_1));
			int threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}
	}

	void VulkanCmdThreadPool::ThreadFunc(uint32_t threadid)
	{
		SPICES_PROFILE_ZONE;

		auto lastTime = std::chrono::high_resolution_clock().now();

		for (;;)
		{
			Task task;
			VkCommandBuffer cmdBuffer = VK_NULL_HANDLE;
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
				cmdBuffer = m_CmdBuffers[FrameInfo::Get().m_FrameIndex][threadid];

				--m_IdleThreadSize;
				m_NotFull.notify_all();
				if (m_TaskQueue.size() > 0) m_NotEmpty.notify_all();
			}

			/**
			* @brief execute task.
			*/
			if (task != nullptr && cmdBuffer != VK_NULL_HANDLE)
			{
				task(cmdBuffer);
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