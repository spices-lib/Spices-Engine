#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {

	class VulkanBuffer : public VulkanObject
	{
	public:
		VulkanBuffer(VulkanState& vulkanState) : VulkanObject(vulkanState) {};
		VulkanBuffer(VulkanState& vulkanState, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		virtual ~VulkanBuffer();

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		inline VkDeviceMemory& GetMomory() { return m_BufferMemory; };
		inline VkBuffer& Get() { return m_Buffer; };
		void Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
		VkDescriptorBufferInfo GetBufferInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
	private:
		void CreateBuffer(VulkanState& vulkanState, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	private:
		VkDeviceSize m_DeviceSize;
		VkBufferUsageFlags m_Uasge;
		VkMemoryPropertyFlags m_Flags;

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

		void* m_LocalMemory = nullptr;
	};
}