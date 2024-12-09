/**
* @file VulkanThreadQueue.cpp.
* @brief The VulkanThreadQueue Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanThreadQueue.h"

namespace Spices {

	std::array<std::shared_ptr<VulkanThreadQueue>, NThreadQueue> VulkanThreadQueue::m_GraphicQueues;
	std::array<std::shared_ptr<VulkanThreadQueue>, NThreadQueue> VulkanThreadQueue::m_ComputeQueues;

	VulkanThreadQueue::VulkanThreadQueue(VulkanState& vulkanState, VkQueue queue)
		: VulkanObject(vulkanState)
		, m_Queue(queue)
		, m_IsInUse(false)
	{}

	VulkanThreadQueue::~VulkanThreadQueue()
	{}

	void VulkanThreadQueue::CreateGraphic(VulkanState & vulkanState, VkQueue queue, uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		m_GraphicQueues[index] = std::make_shared<VulkanThreadQueue>(vulkanState, queue);
	}

	void VulkanThreadQueue::CreateCompute(VulkanState& vulkanState, VkQueue queue, uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		m_ComputeQueues[index] = std::make_shared<VulkanThreadQueue>(vulkanState, queue);
	}

	VulkanThreadQueue& VulkanThreadQueue::FetchGraphicQueue()
	{
		SPICES_PROFILE_ZONE;

		for(;;)
		{
			for (auto& queue : m_GraphicQueues)
			{
				if (!queue->m_IsInUse.load())
				{
					queue->m_IsInUse = true;
					return *queue;
				}
			}
		}
	}

	VulkanThreadQueue& VulkanThreadQueue::FetchComputeQueue()
	{
		SPICES_PROFILE_ZONE;

		for (;;)
		{
			for (auto& queue : m_ComputeQueues)
			{
				if (!queue->m_IsInUse.load())
				{
					queue->m_IsInUse = true;
					return *queue;
				}
			}
		}
	}

	void VulkanThreadQueue::Submit(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instanced a VkSubmitInfo with default value.
		*/
		VkSubmitInfo                     submitInfo{};
		submitInfo.sType               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount  = 1;
		submitInfo.pCommandBuffers     = &commandBuffer;

		VK_CHECK(vkQueueSubmit(m_Queue, 1, &submitInfo, VK_NULL_HANDLE));
	}

	void VulkanThreadQueue::Wait()
	{
		SPICES_PROFILE_ZONE;

		VK_CHECK(vkQueueWaitIdle(m_Queue));

		m_IsInUse = false;
	}
}