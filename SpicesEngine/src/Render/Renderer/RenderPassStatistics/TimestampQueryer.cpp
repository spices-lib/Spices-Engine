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
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_TIMESTAMP, 2);
	}

	void TimestampQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->BeginQuery(commandBuffer, 0);
	}

	void TimestampQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->EndQuery(commandBuffer, 1);
	}

	void TimestampQueryer::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

		uint64_t result[2] = {};
		m_QueryPool->QueryResults(result);

		// timestampPeriod is the number of nanoseconds per timestamp value increment.
		const double microsecondsPerTick = 1e-3f * VulkanDevice::GetDeviceProperties().limits.timestampPeriod;

		const float ms = float(microsecondsPerTick * (double)(result[1] - result[0]));
		std::cout << "ms: " << ms << std::endl;
	}
}