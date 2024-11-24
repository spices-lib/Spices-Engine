#include "Pchheader.h"
#include "TimestampQueryer.h"

namespace Spices {

	TimestampQueryer::TimestampQueryer(VulkanState& state)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_TIMESTAMP);
	}

	void TimestampQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->BeginQuery(commandBuffer);
	}

	void TimestampQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->EndQuery(commandBuffer);
	}

	void TimestampQueryer::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

		uint64_t result;
		m_QueryPool->QueryResults(&result);
	}
}