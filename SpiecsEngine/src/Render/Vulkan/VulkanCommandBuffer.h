/**
* @file VulkanCommandBuffer.h.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spiecs {
	
	/**
	* @brief VulkanCommandPool Class.
	* This class defines the VulkanCommandPool behaver.
	* This class is just a wapper of VkCommandPool.
	*/
	class VulkanCommandPool : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkCommandPool.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] graphicqueuefamily graphic queue identify.
		*/
		VulkanCommandPool(VulkanState& vulkanState, uint32_t graphicqueuefamily);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanCommandPool();
	};

	/**
	* @brief VulkanCommandBuffer Class.
	* This class defines the VulkanCommandBuffer behaver.
	* This class is just a wapper of VkCommandBuffer.
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
		* VkCommanBuffer is created by VkCommandPool, we do not need destroy it here manually.
		*/
		virtual ~VulkanCommandBuffer() {};

		/**
		* @brief Create a new commandbuffer and record custom cmd, submit to graphic queue, execute it immediatelly.
		* @param[in] T The function pointer of what cmd need to execute.
		* @param[in] vulkanState The global VulkanState.
		*/
		template<typename T>
		static void CustomCmd(VulkanState& vulkanState, T func);
	};
	
	template<typename T>
	inline void VulkanCommandBuffer::CustomCmd(VulkanState& vulkanState, T func)
	{
		/**
		* @brief Instanced a VkCommandBufferAllocateInfo with default value.
		*/
		VkCommandBufferAllocateInfo allocInfo{};

		/**
		* @brief Fill in sType.
		*/
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

		/**
		* @brief Fill in levels.
		*/
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		/**
		* @brief Fill in commandPool.
		*/
		allocInfo.commandPool = vulkanState.m_CommandPool;

		/**
		* @brief Fill in commandBufferCount.
		*/
		allocInfo.commandBufferCount = 1;

		/**
		* @brief Allocate s new CommandBuffer from CommandPool.
		*/
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, &commandBuffer);


		/**
		* @brief Instanced a VkCommandBufferBeginInfo with default value.
		*/
		VkCommandBufferBeginInfo beginInfo{};

		/**
		* @brief Fill in sType.
		*/
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		/**
		* @brief Fill in flags.
		*/
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		/**
		* @brief Begin a CommandBuffer
		*/
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		/**
		* @brief Execute function pointer.
		*/
		func(commandBuffer);

		/**
		* @brief End a CommandBuffer
		*/
		vkEndCommandBuffer(commandBuffer);


		/**
		* @brief Instanced a VkSubmitInfo with default value.
		*/
		VkSubmitInfo submitInfo{};

		/**
		* @brief Fill in sType.
		*/
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		/**
		* @brief Fill in commandBufferCount.
		*/
		submitInfo.commandBufferCount = 1;

		/**
		* @brief Fill in pCommandBuffers.
		*/
		submitInfo.pCommandBuffers = &commandBuffer;

		/**
		* @brief Submit the CommandBuffer in graphic Queue.
		*/
		vkQueueSubmit(vulkanState.m_GraphicQueue, 1, &submitInfo, VK_NULL_HANDLE);

		/**
		* @brief Wait for queu execute.
		*/
		vkQueueWaitIdle(vulkanState.m_GraphicQueue);

		/**
		* @brief Free the CommandBuffer that created.
		*/
		vkFreeCommandBuffers(vulkanState.m_Device, vulkanState.m_CommandPool, 1, &commandBuffer);
	}
}
