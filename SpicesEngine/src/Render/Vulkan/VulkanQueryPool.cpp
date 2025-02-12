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
		uint32_t                      stride      ,
		VkQueryPipelineStatisticFlags statistics
	)
		: VulkanObject(vulkanState)
		, m_QueryType(type)
		, m_QueryCount(count)
		, m_Stride(stride)
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

	void VulkanQueryPool::BeginQuery(VkCommandBuffer commandBuffer, uint32_t index) const
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

		assert(!(m_QueryType & VK_QUERY_TYPE_TIMESTAMP));

		/**
		* @brief Begin Query.
		*/
		vkCmdBeginQuery(commandBuffer, m_QueryPool, index, flags);
	}

	void VulkanQueryPool::EndQuery(VkCommandBuffer commandBuffer, uint32_t index) const
	{
		SPICES_PROFILE_ZONE;

		assert(!(m_QueryType & VK_QUERY_TYPE_TIMESTAMP));

		/**
		* @brief EndQuery.
		*/
		vkCmdEndQuery(commandBuffer, m_QueryPool, index);
	}

	void VulkanQueryPool::WriteTimeStamp(VkCommandBuffer commandBuffer, uint32_t index) const
	{
		SPICES_PROFILE_ZONE;

		assert(m_QueryType & VK_QUERY_TYPE_TIMESTAMP);

		vkCmdWriteTimestamp2(commandBuffer, VK_PIPELINE_STAGE_2_NONE, m_QueryPool, index);
	}

	void VulkanQueryPool::Reset(VkCommandBuffer commandBuffer) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Reset query pool.
	    * Must be done outside of render pass.
		*/
		vkCmdResetQueryPool(commandBuffer, m_QueryPool, 0, m_QueryCount);
	}
	
	void VulkanQueryPool::QueryResults(uint64_t* result) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Query all result with uint64_t.
		*/
		VK_CHECK(vkGetQueryPoolResults(
			m_VulkanState.m_Device                     ,
			m_QueryPool                                ,
			0                                          ,
			m_QueryCount                               ,
			m_Stride * m_QueryCount + sizeof(uint64_t) ,
			result                                     ,
			m_Stride                                   ,
			// Store results a 64 bit values and wait until the results have been finished
			// If you don't want to wait, you can use VK_QUERY_RESULT_WITH_AVAILABILITY_BIT
			// which also returns the state of the result (ready) in the result
			VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT
		))
	}
}