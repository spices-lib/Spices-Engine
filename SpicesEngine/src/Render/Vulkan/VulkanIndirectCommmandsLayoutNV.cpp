/**
* @file VulkanIndirectCommmandsLayoutNV.cpp.
* @brief The VulkanIndirectCommmandsLayoutNV Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanIndirectCommmandsLayoutNV.h"

namespace Spices {

	VulkanIndirectCommmandsLayoutNV::VulkanIndirectCommmandsLayoutNV(
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

	VulkanIndirectCommmandsLayoutNV::~VulkanIndirectCommmandsLayoutNV()
	{
		SPICES_PROFILE_ZONE;

		m_VulkanState.m_VkFunc.vkDestroyIndirectCommandsLayoutNV(m_VulkanState.m_Device, m_IndirectCmdsLayout, nullptr);
	}
}