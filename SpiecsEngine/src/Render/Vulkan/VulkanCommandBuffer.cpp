#include "pchheader.h"
#include "VulkanCommandBuffer.h"

namespace Spiecs {

	VulkanCommandPool::VulkanCommandPool(VulkanState& vulkanState, uint32_t graphicqueuefamily)
		: VulkanObject(vulkanState)
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = graphicqueuefamily;

		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_CommandPool));
		SPIECS_LOG("VkCommandPool created succeed!!!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_VulkanState.m_Device, m_VulkanState.m_CommandPool, nullptr);
	}

	VulkanCommandBuffer::VulkanCommandBuffer(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		vulkanState.m_CommandBuffer.resize(MaxFrameInFlight);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = vulkanState.m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(vulkanState.m_CommandBuffer.size());

		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_CommandBuffer.data()));
		SPIECS_LOG("VkCommandBuffer created succeed!!!");
	}

	

}