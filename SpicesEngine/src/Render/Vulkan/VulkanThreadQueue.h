/**
* @file VulkanThreadQueue.h.
* @brief The VulkanThreadQueue Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief Thread Queue Count.
	*/
	static constexpr int NThreadQueue = 4;

	/**
	* @brief VulkanThreadQueue Class.
	* This class is a wapper of Thread VkQueue.
	*/
	class VulkanThreadQueue : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] queue VkQueue.
		*/
		explicit VulkanThreadQueue(VulkanState& vulkanState, VkQueue queue);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanThreadQueue();

		/**
		* @brief Create Graphic VulkanThreadQueue.
		* @param[in] vulkanState VulkanState.
		* @param[in] queue VkQueue.
		* @param[in] index Thread Queue index.
		*/
		static void CreateGraphic(VulkanState& vulkanState, VkQueue queue, uint32_t index);

		/**
		* @brief Create Compute VulkanThreadQueue.
		* @param[in] vulkanState VulkanState.
		* @param[in] queue VkQueue.
		* @param[in] index Thread Queue index.
		*/
		static void CreateCompute(VulkanState& vulkanState, VkQueue queue, uint32_t index);

		/**
		* @brief Fetch valid Graphic Queue.
		* @return Returns valid VulkanThreadQueue.
		*/
		static VulkanThreadQueue& FetchGraphicQueue();

		/**
		* @brief Fetch valid Compute Queue.
		* @return Returns valid VulkanThreadQueue.
		*/
		static VulkanThreadQueue& FetchComputeQueue();

		/**
		* @brief Submit the CommandBuffer in this Queue.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void Submit(VkCommandBuffer commandBuffer);

		/**
		* @brief Wait for queu execute.
		*/
		void Wait();

	private:

		/**
		* @brief Graphic VulkanThreadQueue.
		*/
		static std::array<std::shared_ptr<VulkanThreadQueue>, NThreadQueue> m_GraphicQueues;

		/**
		* @brief Compute VulkanThreadQueue.
		*/
		static std::array<std::shared_ptr<VulkanThreadQueue>, NThreadQueue> m_ComputeQueues;

		/**
		* @brief This Thread VkQueue.
		*/
		VkQueue m_Queue;

		/**
		* @brief True if this VkQueue is in used.
		*/
		std::atomic_bool m_IsInUse;
	};
}