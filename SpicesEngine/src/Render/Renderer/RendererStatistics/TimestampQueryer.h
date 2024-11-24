#pragma once
#include "Core/Core.h"
#include "Queryer.h"

namespace Spices {

	class TimestampQueryer : public Queryer
	{
	public:

		explicit TimestampQueryer(VulkanState& state);
		virtual ~TimestampQueryer() = default;

		virtual void BeginQuery(VkCommandBuffer commandBuffer) override;
		virtual void EndQuery(VkCommandBuffer commandBuffer) override;

		void GetPoolResult();

	private:

		std::unique_ptr<VulkanQueryPool> m_QueryPool;
	};
}