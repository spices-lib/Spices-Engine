/**
* @file RendererStatistics.h
* @brief The RendererStatistics Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanQueryPool.h"

namespace Spices {

	

	class RendererStatistics
	{
	public:

		RendererStatistics(const std::string& name, VulkanState& state);
			
		void BeginStatistics(VkCommandBuffer commandBuffer);
		void EndStatistics(VkCommandBuffer commandBuffer);

		void GetPoolResult();

		virtual ~RendererStatistics() = default;

	private:

		std::string m_Name;
		std::unique_ptr<VulkanQueryPool> m_TimeQueryPool;
		
	};
}
