/**
* @file VulkanQueryPool.h.
* @brief The VulkanQueryPool Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief This Class is a Wrapper of VulkanQueryPool.
	*/
	class VulkanQueryPool : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkQueryPool.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] type VkQueryType.
		* @param[in] count QueryCount.
		* @param[in] statistics Pipeline Query Statistics.
		*/
		VulkanQueryPool(
			VulkanState&                  vulkanState   , 
			VkQueryType                   type          , 
			uint32_t                      count = 1     , 
			VkQueryPipelineStatisticFlags statistics = 0
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanQueryPool() override;

		/**
		* @brief Begin to query next commands.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] index QueryIndex.
		*/
		void BeginQuery(VkCommandBuffer commandBuffer, uint32_t index = 0);

		/**
		* @brief End query previous commands.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] index QueryIndex.
		*/
		void EndQuery(VkCommandBuffer commandBuffer, uint32_t index = 0);

		/**
		* @brief Reset QueryPool.
		* Call it on frame start.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void Reset(VkCommandBuffer commandBuffer);

		/**
		* @brief Query results from Pool.
		* @param[in,out] result Result Data.
		*/
		void QueryResults(uint64_t* result);

		/**
		* @brief Get VkQueryPool Handler.
		* @return Returns VkQueryPool Handler.
		*/
		VkQueryPool Get() { return m_QueryPool; };

	private:

		/**
		* @brief VkQueryPool Handler.
		*/
		VkQueryPool m_QueryPool;

		/**
		* @brief Query Type.
		*/
		VkQueryType m_QueryType;

		/**
		* @brief Query Count.
		*/
		uint32_t m_QueryCount;
	};
}