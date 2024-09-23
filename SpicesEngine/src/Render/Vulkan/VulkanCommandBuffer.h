/**
* @file VulkanCommandBuffer.h.
* @brief The VulkanCommandPool Class and VulkanCommandBuffer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

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
		allocInfo.commandPool         = vulkanState.m_GraphicCommandPool;
		allocInfo.commandBufferCount  = 1;

		/**
		* @brief Allocate s new CommandBuffer from CommandPool.
		*/
		VkCommandBuffer commandBuffer;
		VK_CHECK(vkAllocateCommandBuffers(vulkanState.m_Device, &allocInfo, &commandBuffer));
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_COMMAND_BUFFER, (uint64_t)commandBuffer, vulkanState.m_Device, "CustomCmd Command Buffer")

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

		/**
		* @brief Execute function pointer.
		*/
		func(commandBuffer);

		/**
		* @brief End a CommandBuffer
		*/
		VK_CHECK(vkEndCommandBuffer(commandBuffer));

		/**
		* @brief Instanced a VkSubmitInfo with default value.
		*/
		VkSubmitInfo submitInfo{};
		submitInfo.sType               = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount  = 1;
		submitInfo.pCommandBuffers     = &commandBuffer;

		/**
		* @brief Submit the CommandBuffer in graphic Queue.
		*/
		VK_CHECK(vkQueueSubmit(vulkanState.m_GraphicQueue, 1, &submitInfo, VK_NULL_HANDLE));

		/**
		* @brief Wait for queu execute.
		*/
		VK_CHECK(vkQueueWaitIdle(vulkanState.m_GraphicQueue));

		/**
		* @brief Free the CommandBuffer that created.
		*/
		vkFreeCommandBuffers(vulkanState.m_Device, vulkanState.m_GraphicCommandPool, 1, &commandBuffer);
	}
}
