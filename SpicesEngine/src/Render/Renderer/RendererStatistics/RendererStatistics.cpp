/**
* @file RendererStatistics.cpp
* @brief The RendererStatistics Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RendererStatistics.h"

namespace Spices {

	RendererStatistics::RendererStatistics(const std::string& name, VulkanState& state)
		: m_Name(name)
	{
		SPICES_PROFILE_ZONE;


	}

	void RendererStatistics::BeginStatistics(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;


	}

	void RendererStatistics::EndStatistics(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

	}

	void RendererStatistics::GetPoolResult()
	{
		SPICES_PROFILE_ZONE;

	}
}