/**
* @file VulkanQueryPool.cpp.
* @brief The VulkanQueryPool Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanQueryPool.h"

namespace Spices {

	VulkanQueryPool::VulkanQueryPool(
		VulkanState&                  vulkanState , 
		VkQueryType                   type        , 
		uint32_t                      count       , 
		VkQueryPipelineStatisticFlags statistics
	)
		: VulkanObject(vulkanState)
		, m_QueryType(type)
		, m_QueryCount(count)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Instance a VkQueryPoolCreateInfo.
		*/
		VkQueryPoolCreateInfo             createInfo{};
		createInfo.sType                = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		createInfo.queryType            = type;
		createInfo.queryCount           = count;

		createInfo.flags                = 0;
		createInfo.pipelineStatistics   = statistics;
		createInfo.pNext                = nullptr;

		/**
		* @brief Create Query Pool.
		*/
		VK_CHECK(vkCreateQueryPool(vulkanState.m_Device, &createInfo, nullptr, &m_QueryPool))
		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_QUERY_POOL, (uint64_t)m_QueryPool, m_VulkanState.m_Device, "Query Pool")
	}

	VulkanQueryPool::~VulkanQueryPool()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Destroy this QueryPool.
		*/
		vkDestroyQueryPool(m_VulkanState.m_Device, m_QueryPool, nullptr);
	}

	void VulkanQueryPool::BeginQuery(VkCommandBuffer commandBuffer, uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Determine VkQueryControlFlags.
		*/
		VkQueryControlFlags flags = 0;
		if (m_QueryType & VK_QUERY_TYPE_OCCLUSION)
		{
			flags |= VK_QUERY_CONTROL_PRECISE_BIT;
		}

		/**
		* @brief Begin Query.
		*/
		vkCmdBeginQuery(commandBuffer, m_QueryPool, index, flags);
	}

	void VulkanQueryPool::EndQuery(VkCommandBuffer commandBuffer, uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief EndQuery.
		*/
		vkCmdEndQuery(commandBuffer, m_QueryPool, index);
	}

	void VulkanQueryPool::Reset(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Reset query pool.
	    * Must be done outside of render pass.
		*/
		vkCmdResetQueryPool(commandBuffer, m_QueryPool, 0, m_QueryCount);
	}
	
	void VulkanQueryPool::QueryResults(uint64_t* result)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Query all result with uint64_t.
		*/
		VK_CHECK(vkGetQueryPoolResults(
			m_VulkanState.m_Device          ,
			m_QueryPool                     ,
			0                               ,
			m_QueryCount                    ,
			sizeof(uint64_t) * m_QueryCount ,
			result                          ,
			sizeof(uint64_t)                ,
			// Store results a 64 bit values and wait until the results have been finished
			// If you don't want to wait, you can use VK_QUERY_RESULT_WITH_AVAILABILITY_BIT
			// which also returns the state of the result (ready) in the result
			VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT
		))
	}
}