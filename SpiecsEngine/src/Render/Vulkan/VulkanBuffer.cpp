/**
* @file VulkanBuffer.cpp.
* @brief The VulkanBuffer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"

namespace Spiecs {

	VulkanBuffer::VulkanBuffer(
		VulkanState&          vulkanState  , 
		VkDeviceSize          size         , 
		VkBufferUsageFlags    usage        , 
		VkMemoryPropertyFlags properties
	)
		: VulkanObject(vulkanState)
		, m_DeviceSize(size       )
		, m_Uasge     (usage      )
		, m_Flags     (properties )
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Create a buffer.
		*/
		CreateBuffer(vulkanState, size, usage, properties);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Destroy Vkbuffer.
		*/
		vkDestroyBuffer(m_VulkanState.m_Device, m_Buffer, nullptr);
		vkFreeMemory(m_VulkanState.m_Device, m_BufferMemory, nullptr);
	}

	void VulkanBuffer::CopyBuffer(
		VkBuffer     srcBuffer  , 
		VkBuffer     dstBuffer  , 
		VkDeviceSize size
	)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Use CustomCmd.
		*/
		VulkanCommandBuffer::CustomCmd(m_VulkanState, [&](auto& commandBuffer) {

			/**
			* @brief Instance a VkBufferCopy.
			*/
			VkBufferCopy copyRegion{};
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = size;

			/**
			* @brief Copy Cmd.
			*/
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		});
	}

	VkDeviceAddress& VulkanBuffer::GetAddress()
	{
		if (m_Uasge & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
			return m_BufferAddress;
		}

		SPIECS_CORE_ERROR("This Buffer Cannot Get Address");
	}

	void VulkanBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Map buffer's video memory to local memory.
		*/
		VK_CHECK(vkMapMemory(m_VulkanState.m_Device, m_BufferMemory, offset, size, 0, &m_LocalMemory));
	}

	VkDescriptorBufferInfo* VulkanBuffer::GetBufferInfo(VkDeviceSize size, VkDeviceSize offset)
	{
		SPIECS_PROFILE_ZONE;

		m_BufferInfo.buffer = m_Buffer;
		m_BufferInfo.offset = offset;
		m_BufferInfo.range = size;

		return &m_BufferInfo;
	}

	void VulkanBuffer::WriteToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		SPIECS_PROFILE_ZONE;

		/**
		* @brief Use memcpy copy data.
		*/
		if (size == VK_WHOLE_SIZE) 
		{
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
		SPIECS_PROFILE_ZONE;
		
		/**
		* @brief VkMappedMemoryRange.
		*/
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType          = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory         = m_BufferMemory;
		mappedRange.offset         = offset;
		mappedRange.size           = size;

		/**
		* @brief Flush memory to update buffer's data.
		*/
		VK_CHECK(vkFlushMappedMemoryRanges(m_VulkanState.m_Device, 1, &mappedRange));
	}

	void VulkanBuffer::CreateBuffer(VulkanState& vulkanState, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		SPIECS_PROFILE_ZONE;

		m_DeviceSize = size;
		m_Uasge = usage;
		m_Flags = properties;

		/**
		* @brief Instance a VkBufferCreateInfo.
		*/
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType          = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size           = size;
		bufferInfo.usage          = usage;
		bufferInfo.sharingMode    = VK_SHARING_MODE_EXCLUSIVE;

		/**
		* @brief Create a Buffer.
		*/
		VK_CHECK(vkCreateBuffer(vulkanState.m_Device, &bufferInfo, nullptr, &m_Buffer));
		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_BUFFER, m_Buffer, vulkanState.m_Device, "Buffer");

		/**
		* @brief Get Buffer Memory Requirements.
		*/
		VkMemoryDedicatedRequirements dedicatedRegs{};
		dedicatedRegs.sType                               = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;

		VkMemoryRequirements2 memReqs{};
		memReqs.sType                                     = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
		memReqs.pNext                                     = &dedicatedRegs;

		VkBufferMemoryRequirementsInfo2 bufferReqs{};
		bufferReqs.sType                                  = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
		bufferReqs.buffer                                 = m_Buffer;

		vkGetBufferMemoryRequirements2(m_VulkanState.m_Device, &bufferReqs, &memReqs);

		/**
		* @brief Instance a VkMemoryAllocateInfo.
		*/
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReqs.memoryRequirements.size;
		
		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
			/**
			* @brief Allow Buffer Device Address
			*/
			VkMemoryAllocateFlagsInfoKHR flagsInfo{};
			flagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
			flagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;

			allocInfo.pNext = &flagsInfo;
		}

		/**
		* @brief Get video memory Properties.
		*/
		VkPhysicalDeviceMemoryProperties memProperties{};
		vkGetPhysicalDeviceMemoryProperties(vulkanState.m_PhysicalDevice, &memProperties);

		/**
		* @brief Find a suitable memory block.
		*/
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
		{
			if (memReqs.memoryRequirements.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
			{
				allocInfo.memoryTypeIndex = i;
			}
		}

		/**
		* @brief Allocate video memory and bind it.
		*/
		VK_CHECK(vkAllocateMemory(vulkanState.m_Device, &allocInfo, nullptr, &m_BufferMemory));
		VK_CHECK(vkBindBufferMemory(vulkanState.m_Device, m_Buffer, m_BufferMemory, 0));

		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
			/**
			* @brief Instance a VkBufferDeviceAddressInfo.
			*/
			VkBufferDeviceAddressInfo info = {  };
			info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
			info.buffer = m_Buffer;

			/**
			* @brief Get Address and return it.
			*/
			m_BufferAddress = vkGetBufferDeviceAddress(m_VulkanState.m_Device, &info);
		}
	}
}