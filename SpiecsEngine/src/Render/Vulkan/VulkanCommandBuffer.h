#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

#include <functional>

namespace Spiecs {

	class VulkanCommandPool : public VulkanObject
	{
	public:
		VulkanCommandPool(VulkanState& vulkanState, uint32_t graphicqueuefamily);
		virtual ~VulkanCommandPool();
	};

	class VulkanCommandBuffer : public VulkanObject
	{
	public:
		VulkanCommandBuffer(VulkanState& vulkanState);

		template<typename T>
		static void CustomCmd(VulkanState& vulkanState, T func);
	};
	
	template<typename T>
	inline void VulkanCommandBuffer::CustomCmd(VulkanState& vulkanState, T func)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vulkanState.m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		func(commandBuffer);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vulkanState.m_GraphicQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(vulkanState.m_GraphicQueue);

		vkFreeCommandBuffers(vulkanState.m_Device, vulkanState.m_CommandPool, 1, &commandBuffer);
	}
}
