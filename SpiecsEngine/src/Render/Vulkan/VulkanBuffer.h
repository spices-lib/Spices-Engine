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

		VulkanBuffer(const VulkanBuffer&) = delete;
		VulkanBuffer& operator=(const VulkanBuffer&) = delete;

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		inline VkDeviceMemory& GetMomory() { return m_BufferMemory; };
		inline VkBuffer& Get() { return m_Buffer; };

	private:
		void CreateBuffer(VulkanState& vulkanState, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	private:
		VkDeviceSize m_DeviceSize;
		VkBufferUsageFlags m_Uasge;
		VkMemoryPropertyFlags m_Flags;

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
	};
}