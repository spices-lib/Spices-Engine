#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanQueryPool.h"

namespace Spices {

	class Queryer
	{
	public:
		Queryer() = default;
		virtual ~Queryer() = default;

		virtual void BeginQuery(VkCommandBuffer commandBuffer) = 0;
		virtual void EndQuery(VkCommandBuffer commandBuffer) = 0;
	};
}