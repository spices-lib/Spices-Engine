/**
* @file VulkanCmdThreadPool.cpp.
* @brief The VulkanCmdThreadPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanCmdThreadPool.h"
#include "Render/FrameInfo.h"

namespace Spices {

	const int nCmdThreads = glm::min(4, static_cast<int>(0.5 * std::thread::hardware_concurrency()));

	VulkanCmdThreadPool::VulkanCmdThreadPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
		, ThreadPool_Basic()
	{
		SPICES_PROFILE_ZONE;

		{
			for (int i = 0; i < MaxFrameInFlight; i++)
			{
				m_CmdBuffers[i].resize(nCmdThreads);
			}
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
				DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)m_CmdPools[i], vulkanState.m_Device, "ParallelGraphicCommandPool")
			}
		}

		/**
		* @brief Init ThreadPool.
		*/
		{
			SetMode(PoolMode::MODE_FIXED);
			Start(nCmdThreads);
		}
	}

	VulkanCmdThreadPool::~VulkanCmdThreadPool()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy the Vulkan CommandPool Object.
		*/
		for (int i = 0; i < m_NThreads.load(); i++)
		{
			vkDestroyCommandPool(m_VulkanState.m_Device, m_CmdPools[i], nullptr);
		}
	}

	void VulkanCmdThreadPool::Start(int initThreadSize)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolRunning = true;
		m_InitThreadSize = initThreadSize;
		m_IdleThreadSize = initThreadSize;
		m_NThreads = initThreadSize;

		for (uint32_t i = 0; i < m_InitThreadSize; i++)
		{
			auto ptr = std::make_unique<Thread<VkCommandBuffer>>(std::bind(&VulkanCmdThreadPool::ThreadFunc, this, std::placeholders::_1), i);
			uint32_t threadId = ptr->GetId();
			m_Threads.emplace(threadId, std::move(ptr));
			m_Threads[threadId]->Start();
		}
	}

	void VulkanCmdThreadPool::ThreadFunc(Thread<VkCommandBuffer>* thread)
	{
		SPICES_PROFILE_ZONE;

		auto lastTime = std::chrono::high_resolution_clock::now();

		for (;;)
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
							auto now = std::chrono::high_resolution_clock::now();
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
				VkCommandBuffer cmdBuffer = CreateParallelCommandBuffers(thread->GetId());
				task(cmdBuffer);
				thread->SetThreadInTask(false);
				++m_IdleThreadSize;
				m_IdleCond.notify_all();
			}
			else
			{
				thread->SetThreadInTask(false);
				++m_IdleThreadSize;
				m_IdleCond.notify_all();
			}

			lastTime = std::chrono::high_resolution_clock::now();
		}
	}

	VkCommandBuffer VulkanCmdThreadPool::CreateParallelCommandBuffers(uint32_t threadId)
	{
		SPICES_PROFILE_ZONE;

		VkCommandBufferAllocateInfo        allocInfo{};
		allocInfo.sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool            = m_CmdPools[threadId];
		allocInfo.level                  = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocInfo.commandBufferCount     = 1;

		VkCommandBuffer cmdBuffer;

		VK_CHECK(vkAllocateCommandBuffers(m_VulkanState.m_Device, &allocInfo, &cmdBuffer))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)cmdBuffer, m_VulkanState.m_Device, "ParallelGraphicCommandBuffer")
		SPICES_PROFILE_VK_COLLECT(cmdBuffer)

		m_CmdBuffers[FrameInfo::Get().m_FrameIndex][threadId].push_back(cmdBuffer);

		return cmdBuffer;
	}

	void VulkanCmdThreadPool::FreeParallelCommandBuffers(uint32_t frameIndex)
	{
		SPICES_PROFILE_ZONE;

		auto& commandBuffers = m_CmdBuffers[frameIndex];

		for (int i = 0; i < m_NThreads.load(); i++)
		{
			if (commandBuffers[i].size() == 0) continue;

			vkFreeCommandBuffers(m_VulkanState.m_Device, m_CmdPools[i], commandBuffers[i].size(), commandBuffers[i].data());
			commandBuffers[i].clear();
		}
	}
}