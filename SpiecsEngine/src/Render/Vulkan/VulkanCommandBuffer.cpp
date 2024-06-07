/**
* @file VulkanCommandBuffer.cpp.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanCommandBuffer.h"

namespace Spiecs {

	VulkanCommandPool::VulkanCommandPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Instanced a VkCommandPoolCreateInfo with default value.
		*/
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = vulkanState.m_GraphicQueueFamily;

		/**
		* @brief  Create commandpool and set it global. 
		*/
		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_CommandPool));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_COMMAND_POOL, vulkanState.m_CommandPool, vulkanState.m_Device, "SpiecsEngineCommandPool");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy the Vulkan CommandPool Object.
		*/
		vkDestroyCommandPool(m_VulkanState.m_Device, m_VulkanState.m_CommandPool, nullptr);
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief The CommandBuffer's num should be equals to max frams in flight.
		*/
		vulkanState.m_CommandBuffer.resize(MaxFrameInFlight);

		/**
		* @brief Create VkCommandBufferAllocateInfo struct.
		*/
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool           = vulkanState.m_CommandPool;
		allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount    = static_cast<uint32_t>(vulkanState.m_CommandBuffer.size());

		/**
		* @brief  Create commandbuffer and set it global.
		*/
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_CommandBuffer.data()));

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_COMMAND_BUFFER, vulkanState.m_CommandBuffer[i], vulkanState.m_Device, "SpiecsEngineCommandBuffer");
		}
	}
}