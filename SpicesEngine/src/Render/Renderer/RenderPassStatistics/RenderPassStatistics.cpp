/**
* @file RenderPassStatistics.cpp
* @brief The RenderPassStatistics Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RenderPassStatistics.h"
#include "TimestampQuerier.h"
#include "PipelineStatisticsQuerier.h"

#include <glm/gtc/integer.hpp>

namespace Spices {

	RenderPassStatistics::RenderPassStatistics(VulkanState& state, Querier::StatisticsFlags flags)
	{
		SPICES_PROFILE_ZONE;

		if (flags & Querier::Timestamp)
		{
			m_Queries[glm::log2((int)Querier::Timestamp)] = std::make_unique<TimestampQuerier>(state);
		}

		if (flags & Querier::Pipeline)
		{
			m_Queries[glm::log2((int)Querier::Pipeline)] = std::make_unique<PipelineStatisticsQuerier>(state);
		}
	}

	void RenderPassStatistics::BeginStatistics(VkCommandBuffer commandBuffer, Querier::StatisticsFlags flags) const
	{
		SPICES_PROFILE_ZONE;

		for (auto& queryer : m_Queries)
		{
			if (!queryer) continue;

			if (queryer->GetStatisticsType() & flags)
			{
				queryer->BeginQuery(commandBuffer);
			}
		}
	}

	void RenderPassStatistics::EndStatistics(VkCommandBuffer commandBuffer, Querier::StatisticsFlags flags) const
	{
		SPICES_PROFILE_ZONE;

		for (const auto& querier : m_Queries)
		{
			if (!querier) continue;

			if (querier->GetStatisticsType() & flags)
			{
				querier->EndQuery(commandBuffer);
			}
		}
	}

	void RenderPassStatistics::StoreStatistics() const
	{
		SPICES_PROFILE_ZONE;

		for (auto& querier : m_Queries)
		{
			if (querier) querier->StorePoolResult();
		}
	}
}