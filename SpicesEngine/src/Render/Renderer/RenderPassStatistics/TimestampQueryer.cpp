/**
* @file TimestampQueryer.cpp.
* @brief The TimestampQuerier Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TimestampQuerier.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	TimestampQuerier::TimestampQuerier(VulkanState& state)
		: Querier(StatisticsBits::Timestamp)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_TIMESTAMP, 2);
		m_Result    = std::make_shared<Result>();
	}

	void TimestampQuerier::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->WriteTimeStamp(commandBuffer, 0);
	}

	void TimestampQuerier::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->WriteTimeStamp(commandBuffer, 1);
	}

	void TimestampQuerier::StorePoolResult()
	{
		SPICES_PROFILE_ZONE;

		Result* result = static_cast<Result*>(m_Result.get());

		uint64_t poolResult[3] = {};
		m_QueryPool->QueryResults(poolResult);

		result->valid = poolResult[2];
		result->valid = true;
		if (result->valid)
		{
			// timestampPeriod is the number of nanoseconds per timestamp value increment.
			const float msPerTick = 1e-6f * VulkanDevice::GetDeviceProperties().limits.timestampPeriod;
			result->timeStamp     = msPerTick * (poolResult[1] - poolResult[0]);
		}
	}
}