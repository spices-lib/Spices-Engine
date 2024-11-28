/**
* @file PipelineStatisticsQueryer.cpp.
* @brief The PipelineStatisticsQueryer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PipelineStatisticsQueryer.h"

namespace Spices {

	PipelineStatisticsQueryer::PipelineStatisticsQueryer(VulkanState& state)
		: Queryer(StatisticsBits::Pipeline)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_PIPELINE_STATISTICS, 1, (uint32_t)PipelineStatisticEnum::MAX * sizeof(uint64_t), (VkQueryPipelineStatisticFlags)PipelineStatisticEnum::ALL);
		m_Result    = std::make_shared<Result>();
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

	void PipelineStatisticsQueryer::StorePoolResult()
	{
		SPICES_PROFILE_ZONE;

		Result* result = static_cast<Result*>(m_Result.get());

		uint64_t poolResult[(int)PipelineStatisticEnum::MAX + 1];
		m_QueryPool->QueryResults(poolResult);

		//result->valid = poolResult[(int)PipelineStatisticEnum::MAX];
		result->valid = true;
		if (result->valid)
		{
			memcpy(result->statistics.data(), poolResult, sizeof(result->statistics));
		}
	}
}