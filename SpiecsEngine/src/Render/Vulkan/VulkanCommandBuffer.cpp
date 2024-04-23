/**
* @file VulkanCommandBuffer.cpp.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Definitions.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanCommandBuffer.h"

namespace Spiecs {

	VulkanCommandPool::VulkanCommandPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		/**
		* @brief Instanced a VkCommandPoolCreateInfo with default value.
		*/
		VkCommandPoolCreateInfo poolInfo{};

		/**
		* @brief Fill in sType.
		*/
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		/**
		* @brief Fill in flags.
		*/
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		/**
		* @brief Fill in queueFamilyIndex.
		*/
		poolInfo.queueFamilyIndex = vulkanState.m_GraphicQueueFamily;

		/**
		* @brief  Create commandpool and set it global. 
		*/
		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_CommandPool));
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		/**
		* @brief Destroy the Vulkan CommandPool Object.
		*/
		vkDestroyCommandPool(m_VulkanState.m_Device, m_VulkanState.m_CommandPool, nullptr);
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		/**
		* @brief The CommandBuffer's num should be equals to max frams in flight.
		*/
		vulkanState.m_CommandBuffer.resize(MaxFrameInFlight);

		/**
		* @brief Create VkCommandBufferAllocateInfo struct.
		*/
		VkCommandBufferAllocateInfo allocInfo{};

		/**
		* @brief Fill in sType.
		*/
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

		/**
		* @brief Fill in commandPool.
		*/
		allocInfo.commandPool = vulkanState.m_CommandPool;

		/**
		* @brief Fill in level.
		*/
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		/**
		* @brief Fill in commandBufferCount.
		*/
		allocInfo.commandBufferCount = static_cast<uint32_t>(vulkanState.m_CommandBuffer.size());

		/**
		* @brief  Create commandbuffer and set it global.
		*/
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_CommandBuffer.data()));
	}
}