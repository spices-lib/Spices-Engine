/**
* @file VulkanMemoryAllocator.cpp.
* @brief The VulkanMemoryAllocator Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanMemoryAllocator.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace Spices {

	VulkanMemoryAllocator::VulkanMemoryAllocator(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VmaAllocatorCreateInfo.
		* @see https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/quick_start.html
		*/
		VmaAllocatorCreateInfo                  createInfo {};
		createInfo.instance                   = vulkanState.m_Instance;
		createInfo.physicalDevice             = vulkanState.m_PhysicalDevice;
		createInfo.device                     = vulkanState.m_Device;
		createInfo.vulkanApiVersion           = VK_API_VERSION_1_3;
		createInfo.flags                      = VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT      | 
												VMA_ALLOCATOR_CREATE_KHR_BIND_MEMORY2_BIT              |
												VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE4_BIT              |
												VMA_ALLOCATOR_CREATE_KHR_MAINTENANCE5_BIT              |
												VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT             |
												VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT         |
												VMA_ALLOCATOR_CREATE_EXT_MEMORY_PRIORITY_BIT           |
												VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
												VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT           ;

		/**
		* @brief Create Global Allocator.
		*/
		VK_CHECK(vmaCreateAllocator(&createInfo, &vulkanState.m_VmaAllocator))
	}

	VulkanMemoryAllocator::~VulkanMemoryAllocator()
	{
		SPICES_PROFILE_ZONE;

		vmaDestroyAllocator(m_VulkanState.m_VmaAllocator);
	}
}