/**
* @file PipelineStatisticsQueryer.cpp.
* @brief The PipelineStatisticsQuerier Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PipelineStatisticsQuerier.h"

namespace Spices {

	PipelineStatisticsQuerier::PipelineStatisticsQuerier(VulkanState& state)
		: Querier(StatisticsBits::Pipeline)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(
			state,
			VK_QUERY_TYPE_PIPELINE_STATISTICS,
			1,
			static_cast<uint32_t>(PipelineStatisticEnum::MAX) * sizeof(uint64_t),
			static_cast<VkQueryPipelineStatisticFlags>(PipelineStatisticEnum::ALL)
		);
		
		m_Result = std::make_shared<Result>();
	}

	void PipelineStatisticsQuerier::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->BeginQuery(commandBuffer);
	}

	void PipelineStatisticsQuerier::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->EndQuery(commandBuffer);
	}

	void PipelineStatisticsQuerier::StorePoolResult()
	{
		SPICES_PROFILE_ZONE;

		auto* result = dynamic_cast<Result*>(m_Result.get());

		uint64_t poolResult[static_cast<int>(PipelineStatisticEnum::MAX) + 1];
		m_QueryPool->QueryResults(poolResult);

		//result->valid = poolResult[(int)PipelineStatisticEnum::MAX];
		result->valid = true;
		if (result->valid)
		{
			memcpy(result->statistics.data(), poolResult, sizeof(result->statistics));
		}
	}
}