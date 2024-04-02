#include "pchheader.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"

namespace Spiecs {

	VulkanBuffer::VulkanBuffer(VulkanState& vulkanState, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		: VulkanObject(vulkanState)
		, m_DeviceSize(size)
		, m_Uasge(usage)
		, m_Flags(properties)
	{
		CreateBuffer(vulkanState, size, usage, properties);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		vkDestroyBuffer(m_VulkanState.m_Device, m_Buffer, nullptr);
		vkFreeMemory(m_VulkanState.m_Device, m_BufferMemory, nullptr);
	}

	void VulkanBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VulkanCommandBuffer::CustomCmd(m_VulkanState, [&](auto& commandBuffer) {
			VkBufferCopy copyRegion{};
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		});
	}

	void VulkanBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
	{
		VK_CHECK(vkMapMemory(m_VulkanState.m_Device, m_BufferMemory, offset, size, 0, &m_LocalMemory));
	}

	VkDescriptorBufferInfo VulkanBuffer::GetBufferInfo(VkDeviceSize size, VkDeviceSize offset)
	{
		return VkDescriptorBufferInfo{ m_Buffer , offset, size };
	}

	void VulkanBuffer::WriteToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		if (size == VK_WHOLE_SIZE) {
			memcpy(m_LocalMemory, data, m_DeviceSize);
		}
		else {
			char* memOffset = (char*)m_LocalMemory;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	void VulkanBuffer::Flush(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = m_BufferMemory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		VK_CHECK(vkFlushMappedMemoryRanges(m_VulkanState.m_Device, 1, &mappedRange));
	}

	void VulkanBuffer::CreateBuffer(VulkanState& vulkanState, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		m_DeviceSize = size;
		m_Uasge = usage;
		m_Flags = properties;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK(vkCreateBuffer(vulkanState.m_Device, &bufferInfo, nullptr, &m_Buffer));
		SPIECS_LOG("VkBuffer created succeed!!!");

		// ÄÚ´æ·ÖÅä
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(vulkanState.m_Device, m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(vulkanState.m_PhysicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if (memRequirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				allocInfo.memoryTypeIndex = i;
			}
		}

		VK_CHECK(vkAllocateMemory(vulkanState.m_Device, &allocInfo, nullptr, &m_BufferMemory));
		vkBindBufferMemory(vulkanState.m_Device, m_Buffer, m_BufferMemory, 0);
	}

}