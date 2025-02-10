/**
* @file VulkanThreadQueue.h.
* @brief The VulkanThreadQueue Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Core/Container/ThreadQueue.h"

namespace Spices {

	/**
	* @brief Thread Queue Count.
	*/
	static constexpr int NThreadQueue = 4;

	/**
	* @brief VulkanThreadQueue Class.
	* This class is a wrapper of Thread VkQueue.
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
		virtual ~VulkanThreadQueue() override;

		/**
		* @brief Create Graphic VulkanThreadQueue.
		* @param[in] vulkanState VulkanState.
		* @param[in] queue VkQueue.
		*/
		static void CreateGraphic(VulkanState& vulkanState, VkQueue queue);

		/**
		* @brief Create Compute VulkanThreadQueue.
		* @param[in] vulkanState VulkanState.
		* @param[in] queue VkQueue.
		*/
		static void CreateCompute(VulkanState& vulkanState, VkQueue queue);

		/**
		* @brief Fetch valid Graphic Queue.
		* @return Returns valid VulkanThreadQueue.
		*/
		static std::shared_ptr<VulkanThreadQueue> FetchGraphicQueue();

		/**
		* @brief Fetch valid Compute Queue.
		* @return Returns valid VulkanThreadQueue.
		*/
		static std::shared_ptr<VulkanThreadQueue> FetchComputeQueue();

		/**
		* @brief Push queue to Graphic ThreadQueue.
		* @param[in] queue VulkanThreadQueue.
		*/
		static void PushToGraphic(std::shared_ptr<VulkanThreadQueue>& queue);

		/**
		* @brief Push queue to Compute ThreadQueue.
		* @param[in] queue VulkanThreadQueue.
		*/
		static void PushToCompute(std::shared_ptr<VulkanThreadQueue>& queue);

		/**
		* @brief Destroy all ThreadQueue.
		*/
		static void Destroy();

		/**
		* @brief Submit the CommandBuffer in this Queue.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void Submit(VkCommandBuffer commandBuffer) const;

		/**
		* @brief Wait for queue execute.
		*/
		void Wait() const;

	private:

		/**
		* @brief Graphic VulkanThreadQueue.
		*/
		static scl::thread_queue<std::shared_ptr<VulkanThreadQueue>> m_GraphicQueues;

		/**
		* @brief Compute VulkanThreadQueue.
		*/
		static scl::thread_queue<std::shared_ptr<VulkanThreadQueue>> m_ComputeQueues;

		/**
		* @brief This Thread VkQueue.
		*/
		VkQueue m_Queue;
	};
}