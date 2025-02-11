/**
* @file VulkanCommandBuffer.h.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanThreadQueue.h"

namespace Spices {
	
	/**
	* @brief VulkanCommandPool Class.
	* This class defines the VulkanCommandPool behaves.
	* This class is just a wrapper of VkCommandPool.
	*/
	class VulkanCommandPool : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkCommandPool.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanCommandPool(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanCommandPool() override;

		/**
		* @brief Get Thread Graphic VkCommandPool by thread id.
		* @return Returns thread VkCommandPool.
		*/
		static VkCommandPool& GetThreadGraphicCommandPool();

		/**
		* @brief Get Thread Compute VkCommandPool by thread id.
		* @return Returns thread VkCommandPool.
		*/
		static VkCommandPool& GetThreadComputeCommandPool();
		
		/**
		* @brief Thread Graphic VkCommandPool map. 
		*/
		static std::vector<VkCommandPool> m_ThreadGraphicCommandPool;

		/**
		* @brief Thread Compute VkCommandPool map. 
		*/
		static std::vector<VkCommandPool> m_ThreadComputeCommandPool;

		/**
		* @brief True if this Pool is actived.
		*/
		static bool m_IsPoolActive;

		/**
		* @brief Mutex for GraphicCommandPool.
		*/
		static std::mutex m_GraphicCommandPoolMutex;

		/**
		* @brief Mutex for GraphicCommandPool.
		*/
		static std::mutex m_ComputeCommandPoolMutex;
	};

	/**
	* @brief Wrapper of Instance/Delete VkCommandPool in thread.
	*/
	class VulkanCommandPoolThreadWrapper
	{
	public:

		/**
		* @brief Constructor Function.
		*/
		VulkanCommandPoolThreadWrapper() 
			: m_GraphicThreadId(-1)
			, m_ComputeThreadId(-1)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanCommandPoolThreadWrapper();

		/**
		* @brief Get this instance.
		* @return Return this instance.
		*/
		static VulkanCommandPoolThreadWrapper& GetInst();

	public:

		/**
		* @brief Thread Unique Graphic ThreadId.
		*/
		int m_GraphicThreadId;

		/**
		* @brief Thread Unique Compute ThreadId.
		*/
		int m_ComputeThreadId;
	};

	/**
	* @brief VulkanCommandBuffer Class.
	* This class defines the VulkanCommandBuffer behaves.
	* This class is just a wrapper of VkCommandBuffer.
	*/
	class VulkanCommandBuffer : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkCommandBuffer.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanCommandBuffer(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		* VkCommandBuffer is created by VkCommandPool, we do not need destroy it here manually.
		*/
		virtual ~VulkanCommandBuffer() override = default;

		/**
		* @brief Create a new command buffer and record custom cmd, submit to graphic queue, execute it immediately.
		* @param[in] func The function pointer of what cmd need to execute.
		* @param[in] vulkanState The global VulkanState.
		*/
		template<typename T>
		static void CustomGraphicCmd(VulkanState& vulkanState, T func);
	};
	
	template<typename T>
	inline void VulkanCommandBuffer::CustomGraphicCmd(VulkanState& vulkanState, T func)
	{
		SPICES_PROFILE_ZONE;
		
		/**
		* @brief Instanced a VkCommandBufferAllocateInfo with default value.
		*/
		VkCommandBufferAllocateInfo     allocInfo{};
		allocInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level               = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool         = VulkanCommandPool::GetThreadGraphicCommandPool();
		allocInfo.commandBufferCount  = 1;

		/**
		* @brief Allocate s new CommandBuffer from CommandPool.
		*/
		VkCommandBuffer commandBuffer;
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, &commandBuffer));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, reinterpret_cast<uint64_t>(commandBuffer), vulkanState.m_Device, "CustomCmd Command Buffer")
		
		/**
		* @brief Instanced a VkCommandBufferBeginInfo with default value.
		*/
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		/**
		* @brief Begin a CommandBuffer
		*/
		VK_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		SPICES_PROFILE_VK_ZONE(commandBuffer, "CustomCmd Command Buffer")

		/**
		* @brief Execute function pointer.
		*/
		func(commandBuffer);

		/**
		* @brief End a CommandBuffer
		*/
		VK_CHECK(vkEndCommandBuffer(commandBuffer));

		/**
		* @brief Fetch valid Graphic Queue.
		*/
		auto queue = VulkanThreadQueue::FetchGraphicQueue();

		/**
		* @brief Submit commandBuffer in queue.
		*/
		queue->Submit(commandBuffer);

		/**
		* @brief Wait queue finished.
		*/
		queue->Wait();

		/**
		* @brief Push back to Graphic queue.
		*/
		VulkanThreadQueue::PushToGraphic(queue);

		/**
		* @brief Free the CommandBuffer that created.
		*/
		vkFreeCommandBuffers(vulkanState.m_Device, allocInfo.commandPool, 1, &commandBuffer);
	}
}
