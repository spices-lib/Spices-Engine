/**
* @file PerformanceQueryer.cpp.
* @brief The PerformanceQueryer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PerformanceQueryer.h"

namespace Spices {

	PerformanceQueryer::PerformanceQueryer(VulkanState& state)
		: Queryer(StatisticsBits::Pipeline)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_PERFORMANCE_QUERY_KHR, 1);
		m_Result    = std::make_shared<Result>();
	}

	void PerformanceQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->BeginQuery(commandBuffer);
	}

	void PerformanceQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->EndQuery(commandBuffer);
	}

	std::shared_ptr<Queryer::Result> PerformanceQueryer::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

		Result* result = static_cast<Result*>(m_Result.get());



		return m_Result;
	}
}