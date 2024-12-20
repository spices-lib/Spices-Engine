/**
* @file VulkanIndirectCommandsLayoutNV.cpp.
* @brief The VulkanIndirectCommandsLayoutNV Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanIndirectCommandsLayoutNV.h"

namespace Spices {

	VulkanIndirectCommandsLayoutNV::VulkanIndirectCommandsLayoutNV(
		VulkanState&                          vulkanState ,
		VkIndirectCommandsLayoutCreateInfoNV& info
	)
		: VulkanObject(vulkanState)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Create IndirectCommandsLayout.
		*/
		vulkanState.m_VkFunc.vkCreateIndirectCommandsLayoutNV(vulkanState.m_Device, &info, nullptr, &m_IndirectCmdsLayout);
	}

	VulkanIndirectCommandsLayoutNV::~VulkanIndirectCommandsLayoutNV()
	{
		SPICES_PROFILE_ZONE;

		m_VulkanState.m_VkFunc.vkDestroyIndirectCommandsLayoutNV(m_VulkanState.m_Device, m_IndirectCmdsLayout, nullptr);
	}
}