/**
* @file VulkanCmdThreadPool.cpp.
* @brief The VulkanCmdThreadPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanCmdThreadPool.h"
#include "Render/FrameInfo.h"

namespace Spices {

	const int nCmdThreads = std::thread::hardware_concurrency();

	VulkanCmdThreadPool::VulkanCmdThreadPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
		, ThreadPool_Basic()
	{
		SetMode(PoolMode::MODE_FIXED);
		Start(nCmdThreads);

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_CmdBuffers[i].resize(nCmdThreads);
		}

		VkCommandBufferAllocateInfo       allocInfo{};
		allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool           = vulkanState.m_CommandPool;
		allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocInfo.commandBufferCount    = nCmdThreads;

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, m_CmdBuffers[i].data()));

			for (int j = 0; j < nCmdThreads; j++)
			{
				VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_COMMAND_BUFFER, m_CmdBuffers[i][j], vulkanState.m_Device, "ParallelCommandBuffer");
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

		while (1)
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
				task(m_CmdBuffers[FrameInfo::Get().m_FrameIndex][threadid]);
				m_TaskFinish.notify_all();
			}

			++m_IdleThreadSize;

			lastTime = std::chrono::high_resolution_clock().now();
		}
	}
}