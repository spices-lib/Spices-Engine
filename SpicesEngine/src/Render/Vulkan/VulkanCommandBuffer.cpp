/**
* @file VulkanCommandBuffer.cpp.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanCommandBuffer.h"

namespace Spices {

	VulkanCommandPool::VulkanCommandPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instanced a VkCommandPoolCreateInfo with default value.
		*/
		VkCommandPoolCreateInfo     poolInfo{};
		poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = vulkanState.m_GraphicQueueFamily;

		/**
		* @brief Create commandpool and set it global. 
		*/
		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_GraphicCommandPool));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)vulkanState.m_GraphicCommandPool, vulkanState.m_Device, "GraphicCommandPool")

		poolInfo.queueFamilyIndex = vulkanState.m_ComputeQueueFamily;

		/**
		* @brief Create commandpool and set it global.
		*/
		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_ComputeCommandPool));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, (uint64_t)vulkanState.m_ComputeCommandPool, vulkanState.m_Device, "ComputeCommandPool")
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy the Vulkan CommandPool Object.
		*/
		vkDestroyCommandPool(m_VulkanState.m_Device, m_VulkanState.m_ComputeCommandPool, nullptr);
		vkDestroyCommandPool(m_VulkanState.m_Device, m_VulkanState.m_GraphicCommandPool, nullptr);
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create VkCommandBufferAllocateInfo struct.
		*/
		VkCommandBufferAllocateInfo       allocInfo{};
		allocInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool           = vulkanState.m_GraphicCommandPool;
		allocInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount    = MaxFrameInFlight;

		/**
		* @brief  Create commandbuffer and set it global.
		*/
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_GraphicCommandBuffer.data()));

		allocInfo.commandPool = vulkanState.m_ComputeCommandPool;
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_ComputeCommandBuffer.data()));

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)vulkanState.m_GraphicCommandBuffer[i], vulkanState.m_Device, "GraphicCommandBuffer")
			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)vulkanState.m_ComputeCommandBuffer[i], vulkanState.m_Device, "ComputeCommandBuffer")
		}
	}
}