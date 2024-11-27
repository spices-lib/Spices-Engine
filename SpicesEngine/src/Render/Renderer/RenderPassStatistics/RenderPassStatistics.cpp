/**
* @file RenderPassStatistics.cpp
* @brief The RenderPassStatistics Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RenderPassStatistics.h"
#include "TimestampQueryer.h"
#include "PipelineStatisticsQueryer.h"

#include <glm/gtc/integer.hpp>

namespace Spices {

	RenderPassStatistics::RenderPassStatistics(VulkanState& state, Queryer::StatisticsFlags flags)
	{
		SPICES_PROFILE_ZONE;

		if (flags & Queryer::Timestamp)
		{
			m_Queries[glm::log2((int)Queryer::Timestamp)] = std::make_unique<TimestampQueryer>(state);
		}

		if (flags & Queryer::Pipeline)
		{
			m_Queries[glm::log2((int)Queryer::Pipeline)] = std::make_unique<PipelineStatisticsQueryer>(state);
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
}