/**
* @file RenderPassStatistics.cpp
* @brief The RenderPassStatistics Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RenderPassStatistics.h"
#include "TimestampQueryer.h"
#include "PipelineStatisticsQueryer.h"

namespace Spices {

	RenderPassStatistics::RenderPassStatistics(VulkanState& state, StatisticsFlags flags)
	{
		SPICES_PROFILE_ZONE;

		if (flags & StatisticsBits::Timestamp)
		{
			m_Queries[StatisticsBits::Timestamp] = std::make_unique<TimestampQueryer>(state);
		}

		if (flags & StatisticsBits::Pipeline)
		{
			m_Queries[StatisticsBits::Pipeline] = std::make_unique<PipelineStatisticsQueryer>(state);
		}
	}

	void RenderPassStatistics::BeginStatistics(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		for (auto& queryer : m_Queries)
		{
			if(queryer) queryer->BeginQuery(commandBuffer);
		}
	}

	void RenderPassStatistics::EndStatistics(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		for (auto& queryer : m_Queries)
		{
			if (queryer) queryer->EndQuery(commandBuffer);
		}
	}

	void RenderPassStatistics::GetStatisticsResult()
	{
		SPICES_PROFILE_ZONE;

		for (auto& queryer : m_Queries)
		{
			if (queryer) queryer->GetPoolResult();
		}
	}
}