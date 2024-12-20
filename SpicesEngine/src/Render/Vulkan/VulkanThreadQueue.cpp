/**
* @file VulkanThreadQueue.cpp.
* @brief The VulkanThreadQueue Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanThreadQueue.h"

namespace Spices {

	scl::thread_queue<std::shared_ptr<VulkanThreadQueue>> VulkanThreadQueue::m_GraphicQueues;
	scl::thread_queue<std::shared_ptr<VulkanThreadQueue>> VulkanThreadQueue::m_ComputeQueues;

	VulkanThreadQueue::VulkanThreadQueue(VulkanState& vulkanState, VkQueue queue)
		: VulkanObject(vulkanState)
		, m_Queue(queue)
	{}

	VulkanThreadQueue::~VulkanThreadQueue()
	{}

	void VulkanThreadQueue::CreateGraphic(VulkanState & vulkanState, VkQueue queue)
	{
		SPICES_PROFILE_ZONE;

		m_GraphicQueues.Push(std::make_shared<VulkanThreadQueue>(vulkanState, queue));
	}

	void VulkanThreadQueue::CreateCompute(VulkanState& vulkanState, VkQueue queue)
	{
		SPICES_PROFILE_ZONE;

		m_ComputeQueues.Push(std::make_shared<VulkanThreadQueue>(vulkanState, queue));
	}

	std::shared_ptr<VulkanThreadQueue> VulkanThreadQueue::FetchGraphicQueue()
	{
		SPICES_PROFILE_ZONE;

		return m_GraphicQueues.Pop();
	}

	std::shared_ptr<VulkanThreadQueue> VulkanThreadQueue::FetchComputeQueue()
	{
		SPICES_PROFILE_ZONE;

		return m_ComputeQueues.Pop();
	}

	void VulkanThreadQueue::PushToGraphic(std::shared_ptr<VulkanThreadQueue>& queue)
	{
		SPICES_PROFILE_ZONE;

		m_GraphicQueues.Push(std::move(queue));
	}

	void VulkanThreadQueue::PushToCompute(std::shared_ptr<VulkanThreadQueue>& queue)
	{
		SPICES_PROFILE_ZONE;

		m_ComputeQueues.Push(std::move(queue));
	}

	void VulkanThreadQueue::Destroy()
	{
		SPICES_PROFILE_ZONE;

		m_GraphicQueues.Clear();
		m_ComputeQueues.Clear();
	}

	void VulkanThreadQueue::Submit(VkCommandBuffer commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instanced a VkSubmitInfo with default value.
		*/
		VkSubmitInfo                     submitInfo{};
		submitInfo.sType               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount  = 1;
		submitInfo.pCommandBuffers     = &commandBuffer;

		VK_CHECK(vkQueueSubmit(m_Queue, 1, &submitInfo, VK_NULL_HANDLE))
	}

	void VulkanThreadQueue::Wait() const
	{
		SPICES_PROFILE_ZONE;

		VK_CHECK(vkQueueWaitIdle(m_Queue))
	}
}