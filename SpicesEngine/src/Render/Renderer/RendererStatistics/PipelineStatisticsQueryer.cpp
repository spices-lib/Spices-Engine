#include "Pchheader.h"
#include "PipelineStatisticsQueryer.h"

namespace Spices {

	PipelineStatisticsQueryer::PipelineStatisticsQueryer(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		for (int i = 0; i < (int)PipelineStatisticEnum::MAX; i++)
		{
			m_QueryPool[i] = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_PIPELINE_STATISTICS, 1, (VkQueryPipelineStatisticFlagBits)(1 << i));
		}
	}

	void PipelineStatisticsQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		for (int i = 0; i < (int)PipelineStatisticEnum::MAX; i++)
		{
			m_QueryPool[i]->Reset(commandBuffer);
			m_QueryPool[i]->BeginQuery(commandBuffer);
		}
	}

	void PipelineStatisticsQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		for (int i = 0; i < (int)PipelineStatisticEnum::MAX; i++)
		{
			m_QueryPool[i]->EndQuery(commandBuffer);
		}
	}

	void PipelineStatisticsQueryer::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

		
	}
}