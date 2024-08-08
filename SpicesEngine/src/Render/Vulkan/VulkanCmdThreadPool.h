/**
* @file VulkanCmdThreadPool.h.
* @brief The VulkanCmdThreadPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "Core/Thread/ThreadPool.h"
#include "VulkanCommandBuffer.h"

namespace Spices {

	/**
	* @brief VulkanCmdThreadPool Class.
	* This class defines the VulkanCmdThreadPool behaves.
	* Is use for submit commands parallel.
	*/
	class VulkanCmdThreadPool : public VulkanObject, public ThreadPool<void, VkCommandBuffer>
	{
	public:

		/**
		* @brief Constructor Function.
		* Create Specific ThreadPool.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanCmdThreadPool(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanCmdThreadPool() override = default;

		/**
		* @brief Start Run this thread pool.
		* @param[in] initThreadSize Thread Size.
		*/
		virtual void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency()) override;

		/**
		* @brief Submit a task to task queue, and wait for a idle thread to execute it.
		* @tparam func Task Function.
		*/
		void SubmitTask(std::function<void(VkCommandBuffer cmdBuffer)> func);

		/**
		* @brief GetCommandBuffers.
		* @return Return all CommandBuffers.
		*/
		std::vector<VkCommandBuffer>& GetCommandBuffers() { return m_CmdBuffers; }

	protected:

		/**
		* @brief Thread Function.
		* @param[in] threadid Thread id from std
		*/
		virtual void ThreadFunc(uint32_t threadid) override;

	private:

		/**
		* @brief Parallel Secondary CommandBuffers.
		*/
		std::vector<VkCommandBuffer> m_CmdBuffers;
	};
}
