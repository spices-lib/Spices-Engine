/**
* @file PipelineStatisticsQueryer.cpp.
* @brief The PipelineStatisticsQueryer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PipelineStatisticsQueryer.h"

namespace Spices {

	PipelineStatisticsQueryer::PipelineStatisticsQueryer(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_PIPELINE_STATISTICS, 1, (VkQueryPipelineStatisticFlags)PipelineStatisticEnum::ALL);
	}

	void PipelineStatisticsQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->BeginQuery(commandBuffer);
	}

	void PipelineStatisticsQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->EndQuery(commandBuffer);
	}

	void PipelineStatisticsQueryer::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

		uint64_t result[20] = {0};
		m_QueryPool->QueryResults(result);

		std::cout << result << std::endl;
	}

	void PipelineStatisticsQueryer::DrawPoolResult()
	{
		SPICES_PROFILE_ZONE;
	}
}