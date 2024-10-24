/**
* @file VulkanBuffer.cpp.
* @brief The VulkanBuffer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanMemoryAllocator.h"

namespace Spices {

	VulkanBuffer::VulkanBuffer(
		VulkanState&          vulkanState  , 
		const std::string&    name         ,
		VkDeviceSize          size         , 
		VkBufferUsageFlags    usage        , 
		VkMemoryPropertyFlags properties
	)
		: VulkanObject(vulkanState)
		, m_Name      (name       )
		, m_DeviceSize(size       )
		, m_Usage     (usage      )
		, m_Flags     (properties )
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create a buffer.
		*/
		CreateBuffer(vulkanState, name, size, usage, properties);
	}

	VulkanBuffer::~VulkanBuffer()
	{
		SPICES_PROFILE_ZONE;

#ifdef VMA_ALLOCATOR

		/**
		* @brief Destroy Vkbuffer.
		*/
		vmaDestroyBuffer(m_VulkanState.m_VmaAllocator, m_Buffer, m_Alloc);

#else

		if (m_LocalMemory)
		{
			vkUnmapMemory(m_VulkanState.m_Device, m_BufferMemory);
		}

		/**
		* @brief Destroy Vkbuffer.
		*/
		vkDestroyBuffer(m_VulkanState.m_Device, m_Buffer, nullptr);
		vkFreeMemory(m_VulkanState.m_Device, m_BufferMemory, nullptr);

#endif

	}

	void VulkanBuffer::CopyBuffer(
		VkBuffer     srcBuffer  , 
		VkBuffer     dstBuffer  , 
		VkDeviceSize size
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Use CustomCmd.
		*/
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](auto& commandBuffer) {

			/**
			* @brief Instance a VkBufferCopy.
			*/
			VkBufferCopy                                copyRegion{};
			copyRegion.srcOffset                      = 0;
			copyRegion.dstOffset                      = 0;
			copyRegion.size                           = size;

			/**
			* @brief Copy Cmd.
			*/
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		});
	}

	VkDeviceAddress& VulkanBuffer::GetAddress()
	{
		if (m_Usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
			return m_BufferAddress;
		}

		SPICES_CORE_ERROR("This Buffer Cannot Get Address");
		return m_BufferAddress;
	}

	void VulkanBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
	{
		SPICES_PROFILE_ZONE;

#ifdef VMA_ALLOCATOR

		SPICES_CORE_ERROR("VMA cannot enter this function.");

#else

		/**
		* @brief Map buffer's video memory to local memory.
		*/
		VK_CHECK(vkMapMemory(m_VulkanState.m_Device, m_BufferMemory, offset, size, 0, &m_LocalMemory));

#endif

	}

	VkDescriptorBufferInfo* VulkanBuffer::GetBufferInfo(VkDeviceSize size, VkDeviceSize offset)
	{
		SPICES_PROFILE_ZONE;

		m_BufferInfo.buffer   = m_Buffer;
		m_BufferInfo.offset   = offset;
		m_BufferInfo.range    = size;

		return &m_BufferInfo;
	}

	void VulkanBuffer::WriteToBuffer(const void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		SPICES_PROFILE_ZONE;

#ifdef VMA_ALLOCATOR

		/**
		* @brief Maped memory inside.
		*/
		if (size == VK_WHOLE_SIZE)
		{
			VK_CHECK(vmaCopyMemoryToAllocation(m_VulkanState.m_VmaAllocator, data, m_Alloc, offset, m_DeviceSize))
		}
		else
		{
			VK_CHECK(vmaCopyMemoryToAllocation(m_VulkanState.m_VmaAllocator, data, m_Alloc, offset, size))
		}

#else

		if (!m_LocalMemory){ Map(); }

		/**
		* @brief Use memcpy copy data.
		*/
		if (size == VK_WHOLE_SIZE) 
		{
			memcpy(m_LocalMemory, data, m_DeviceSize);
		}
		else 
		{
			char* memOffset = static_cast<char*>(m_LocalMemory);
			memOffset += offset;
			memcpy(memOffset, data, size);
		}

#endif

	}

	void VulkanBuffer::WriteFromBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
	{
		SPICES_PROFILE_ZONE;

#ifdef VMA_ALLOCATOR

		/**
		* @brief Maped memory inside.
		*/
		if (size == VK_WHOLE_SIZE)
		{
			VK_CHECK(vmaCopyAllocationToMemory(m_VulkanState.m_VmaAllocator, m_Alloc, offset, data, m_DeviceSize))
		}
		else
		{
			VK_CHECK(vmaCopyAllocationToMemory(m_VulkanState.m_VmaAllocator, m_Alloc, offset, data, size))
		}

#else

		if (!m_LocalMemory) { Map(); }

		/**
		* @brief Use memcpy copy data.
		*/
		if (size == VK_WHOLE_SIZE)
		{
			memcpy(data, m_LocalMemory, m_DeviceSize);
		}
		else {
			char* memOffset = static_cast<char*>(m_LocalMemory);
			memOffset += offset;
			memcpy(data, memOffset, size);
		}

#endif

	}

	void VulkanBuffer::Flush(VkDeviceSize size, VkDeviceSize offset) const
	{
		SPICES_PROFILE_ZONE;
		
#ifdef VMA_ALLOCATOR

		/**
		* @brief Flush memory to update buffer's data.
		*/
		VK_CHECK(vmaFlushAllocation(m_VulkanState.m_VmaAllocator, m_Alloc, offset, size))

#else

		/**
		* @brief VkMappedMemoryRange.
		*/
		VkMappedMemoryRange                                 mappedRange {};
		mappedRange.sType                                 = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory                                = m_BufferMemory;
		mappedRange.offset                                = offset;
		mappedRange.size                                  = size;

		/**
		* @brief Flush memory to update buffer's data.
		*/
		VK_CHECK(vkFlushMappedMemoryRanges(m_VulkanState.m_Device, 1, &mappedRange))

#endif
		
	}

	void VulkanBuffer::CreateBuffer(
		VulkanState&          vulkanState , 
		const std::string&    name        , 
		VkDeviceSize          size        , 
		VkBufferUsageFlags    usage       , 
		VkMemoryPropertyFlags properties
	)
	{
		SPICES_PROFILE_ZONE;

		m_Name       = name;
		m_DeviceSize = size;
		m_Usage      = usage;
		m_Flags      = properties;

		/**
		* @brief Instance a VkBufferCreateInfo.
		*/
		VkBufferCreateInfo                                  bufferInfo{};
		bufferInfo.sType                                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size                                   = size;
		bufferInfo.usage                                  = usage;
		bufferInfo.sharingMode                            = VK_SHARING_MODE_EXCLUSIVE;

#ifdef VMA_ALLOCATOR

		/**
		* @brief Instance a VmaAllocationCreateInfo.
		*/
		VmaAllocationCreateInfo                             allocInfo{};
		allocInfo.usage                                   = VMA_MEMORY_USAGE_AUTO;

		if (properties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		{
			allocInfo.flags                             |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		}

		if (properties & VMA_MEMORY_PROPERTY_DEDICATED_MEMORY_BIT)
		{
			allocInfo.flags                             |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		}

		/**
		* @brief Create a Buffer.
		*/
		VK_CHECK(vmaCreateBuffer(vulkanState.m_VmaAllocator, &bufferInfo, &allocInfo, &m_Buffer, &m_Alloc, nullptr))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)m_Buffer, vulkanState.m_Device, name)

		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
			/**
			* @brief Instance a VkBufferDeviceAddressInfo.
			*/
			VkBufferDeviceAddressInfo           info {};
			info.sType                        = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
			info.buffer                       = m_Buffer;

			/**
			* @brief Get Address and return it.
			*/
			m_BufferAddress = vkGetBufferDeviceAddress(m_VulkanState.m_Device, &info);
		}

#else

		/**
		* @brief Create a Buffer.
		*/
		VK_CHECK(vkCreateBuffer(vulkanState.m_Device, &bufferInfo, nullptr, &m_Buffer));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)m_Buffer, vulkanState.m_Device, "Buffer")

		/**
		* @brief Get Buffer Memory Requirements.
		*/
		VkMemoryDedicatedRequirements                       dedicatedRegs{};
		dedicatedRegs.sType                               = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS;

		VkMemoryRequirements2                               memReqs{};
		memReqs.sType                                     = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
		memReqs.pNext                                     = &dedicatedRegs;

		VkBufferMemoryRequirementsInfo2                     bufferReqs{};
		bufferReqs.sType                                  = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
		bufferReqs.buffer                                 = m_Buffer;

		vkGetBufferMemoryRequirements2(m_VulkanState.m_Device, &bufferReqs, &memReqs);

		/**
		* @brief Instance a VkMemoryAllocateInfo.
		*/
		VkMemoryAllocateInfo                                allocInfo{};
		allocInfo.sType                                   = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize                          = memReqs.memoryRequirements.size;
		
		/**
		* @brief Allow Buffer Device Address
		*/
		VkMemoryAllocateFlagsInfoKHR                        flagsInfo{};
		flagsInfo.sType                                   = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO_KHR;
		flagsInfo.flags                                   = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
		{
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
				break;
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
			VkBufferDeviceAddressInfo           info {};
			info.sType                        = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
			info.buffer                       = m_Buffer;

			/**
			* @brief Get Address and return it.
			*/
			m_BufferAddress = vkGetBufferDeviceAddress(m_VulkanState.m_Device, &info);
		}

#endif

	}
}