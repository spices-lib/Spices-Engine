/**
* @file VulkanCommandBuffer.cpp.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderBackend.h"

namespace Spices {
	
	std::vector<VkCommandPool> VulkanCommandPool::m_ThreadGraphicCommandPool;
	std::vector<VkCommandPool> VulkanCommandPool::m_ThreadComputeCommandPool;
	std::mutex VulkanCommandPool::m_GraphicCommandPoolMutex;
	std::mutex VulkanCommandPool::m_ComputeCommandPoolMutex;
	bool VulkanCommandPool::m_IsPoolActive = false;

	VulkanCommandPool::VulkanCommandPool(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolActive = true;

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
		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_GraphicCommandPool))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(vulkanState.m_GraphicCommandPool), vulkanState.m_Device, "GraphicCommandPool")
		
		VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId = 0;
		m_ThreadGraphicCommandPool.push_back(vulkanState.m_GraphicCommandPool);
		
		poolInfo.queueFamilyIndex = vulkanState.m_ComputeQueueFamily;

		/**
		* @brief Create commandpool and set it global.
		*/
		VK_CHECK(vkCreateCommandPool(vulkanState.m_Device, &poolInfo, nullptr, &vulkanState.m_ComputeCommandPool))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(vulkanState.m_ComputeCommandPool), vulkanState.m_Device, "ComputeCommandPool")

		VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId = 0;
		m_ThreadComputeCommandPool.push_back(vulkanState.m_ComputeCommandPool);
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		SPICES_PROFILE_ZONE;

		m_IsPoolActive = false;

		/**
		* @brief Destroy the Vulkan CommandPool Object.
		*/
		for(auto& pool : m_ThreadGraphicCommandPool)
		{
			if (pool)
			{
				vkDestroyCommandPool(m_VulkanState.m_Device, pool, nullptr);
				pool = nullptr;
			}
		}
		
		for(auto& pool : m_ThreadComputeCommandPool)
		{
			if (pool)
			{
				vkDestroyCommandPool(m_VulkanState.m_Device, pool, nullptr);
				pool = nullptr;
			}
		}
	}

	VkCommandPool& VulkanCommandPool::GetThreadGraphicCommandPool()
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_GraphicCommandPoolMutex);

		if (!m_IsPoolActive)
		{
			SPICES_CORE_ERROR("CommandPool is not active.")
		}

		if(VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId == -1)
		{
			for (int i = 0; i < m_ThreadGraphicCommandPool.size(); i++)
			{
				if (!m_ThreadGraphicCommandPool[i]) VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId = i;
			}
			
			if (VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId == -1)
			{
				VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId = m_ThreadGraphicCommandPool.size();
				m_ThreadGraphicCommandPool.push_back(nullptr);
			}
		}
		
		if(!m_ThreadGraphicCommandPool[VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId])
		{
			/**
			* @brief Instanced a VkCommandPoolCreateInfo with default value.
			*/
			VkCommandPoolCreateInfo     poolInfo{};
			poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = VulkanRenderBackend::GetState().m_GraphicQueueFamily;

			/**
			* @brief Create commandpool and set it global. 
			*/
			VkCommandPool pool;
			VK_CHECK(vkCreateCommandPool(VulkanRenderBackend::GetState().m_Device, &poolInfo, nullptr, &pool))
			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(pool), VulkanRenderBackend::GetState().m_Device, "ThreadGraphicCommandPool")
			m_ThreadGraphicCommandPool[VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId] = std::move(pool);
		}

		return m_ThreadGraphicCommandPool[VulkanCommandPoolThreadWrapper::GetInst().m_GraphicThreadId];
	}

	VkCommandPool& VulkanCommandPool::GetThreadComputeCommandPool()
	{
		SPICES_PROFILE_ZONE;

		std::unique_lock<std::mutex> lock(m_ComputeCommandPoolMutex);

		if (!m_IsPoolActive)
		{
			SPICES_CORE_ERROR("CommandPool is not active.")
		}

		if (VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId == -1)
		{
			for (int i = 0; i < m_ThreadComputeCommandPool.size(); i++)
			{
				if (!m_ThreadComputeCommandPool[i]) VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId = i;
			}

			if (VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId == -1)
			{
				VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId = m_ThreadComputeCommandPool.size();
				m_ThreadComputeCommandPool.push_back(nullptr);
			}
		}
		
		if(m_ThreadComputeCommandPool[VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId])
		{
			/**
			* @brief Instanced a VkCommandPoolCreateInfo with default value.
			*/
			VkCommandPoolCreateInfo     poolInfo{};
			poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = VulkanRenderBackend::GetState().m_ComputeQueueFamily;

			/**
			* @brief Create commandpool and set it global. 
			*/
			VkCommandPool pool;
			VK_CHECK(vkCreateCommandPool(VulkanRenderBackend::GetState().m_Device, &poolInfo, nullptr, &pool))
			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(pool), VulkanRenderBackend::GetState().m_Device, "ThreadComputeCommandPool")
			m_ThreadComputeCommandPool[VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId] = std::move(pool);
		}

		return m_ThreadComputeCommandPool[VulkanCommandPoolThreadWrapper::GetInst().m_ComputeThreadId];
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
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_GraphicCommandBuffer.data()))

		allocInfo.commandPool = vulkanState.m_ComputeCommandPool;
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, vulkanState.m_ComputeCommandBuffer.data()))

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, reinterpret_cast<uint64_t>(vulkanState.m_GraphicCommandBuffer[i]), vulkanState.m_Device, "GraphicCommandBuffer")
			DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, reinterpret_cast<uint64_t>(vulkanState.m_ComputeCommandBuffer[i]), vulkanState.m_Device, "ComputeCommandBuffer")

			SPICES_PROFILE_VK_COLLECT(vulkanState.m_GraphicCommandBuffer[i])
			SPICES_PROFILE_VK_COLLECT(vulkanState.m_ComputeCommandBuffer[i])
		}
	}

	VulkanCommandPoolThreadWrapper::~VulkanCommandPoolThreadWrapper()
	{
		SPICES_PROFILE_ZONE;

		if (!VulkanCommandPool::m_IsPoolActive) return;

		if (m_GraphicThreadId >= 0)
		{
			auto& pool = VulkanCommandPool::GetThreadGraphicCommandPool();

			if (pool)
			{
				vkDestroyCommandPool(VulkanRenderBackend::GetState().m_Device, pool, nullptr);
				pool = nullptr;
			}
		}

		if (m_ComputeThreadId >= 0)
		{
			auto& pool = VulkanCommandPool::GetThreadComputeCommandPool();

			if (pool)
			{
				vkDestroyCommandPool(VulkanRenderBackend::GetState().m_Device, pool, nullptr);
				pool = nullptr;
			}
		}
	}

	VulkanCommandPoolThreadWrapper& VulkanCommandPoolThreadWrapper::GetInst()
	{
		/**
		* @brief Thread Unique VulkanCommandPoolThreadWrapper.
		*/
		static _declspec(thread) VulkanCommandPoolThreadWrapper pTLSVulkanCommandPool;

		return pTLSVulkanCommandPool;
	}
}
