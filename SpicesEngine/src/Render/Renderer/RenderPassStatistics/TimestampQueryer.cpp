/**
* @file TimestampQueryer.cpp.
* @brief The TimestampQueryer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "TimestampQueryer.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

namespace Spices {

	TimestampQueryer::TimestampQueryer(VulkanState& state)
		: Queryer(StatisticsBits::Timestamp)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_TIMESTAMP, 2);
		m_Result    = std::make_shared<Result>();
	}

	void TimestampQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->WriteTimeStamp(commandBuffer, 0);
	}

	void TimestampQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->WriteTimeStamp(commandBuffer, 1);
	}

	std::shared_ptr<Queryer::Result> TimestampQueryer::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

		Result* result = static_cast<Result*>(m_Result.get());

		uint64_t poolResult[3] = {};
		m_QueryPool->QueryResults(poolResult);

		result->valid = poolResult[2];
		if (result->valid)
		{
			// timestampPeriod is the number of nanoseconds per timestamp value increment.
			const float msPerTick = 1e-6f * VulkanDevice::GetDeviceProperties().limits.timestampPeriod;
			result->timeStamp     = msPerTick * (poolResult[1] - poolResult[0]);
		}

		return m_Result;
	}
}