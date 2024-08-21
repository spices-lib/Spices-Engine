/**
* @file VulkanIndirectDrawNV.cpp.
* @brief The VulkanIndirectDrawNV Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanIndirectDrawNV.h"

namespace Spices {

	VulkanIndirectDrawNV::VulkanIndirectDrawNV(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
		, indirectCmdsLayout(VK_NULL_HANDLE)
		, inputBuffer(nullptr)
		, preprocessBuffer(nullptr)
		, nMeshPack(0)
		, preprocessSize(0)
	{}

	VulkanIndirectDrawNV::~VulkanIndirectDrawNV()
	{
		SPICES_PROFILE_ZONE;

		if (indirectCmdsLayout)
		{
			m_VulkanState.m_VkFunc.vkDestroyIndirectCommandsLayoutNV(m_VulkanState.m_Device, indirectCmdsLayout, nullptr);
		}
	}

	void VulkanIndirectDrawNV::ResetCommandsLayout()
	{
		SPICES_PROFILE_ZONE;

		inputStrides.clear();
		strides = 0;
		if (indirectCmdsLayout)
		{
			m_VulkanState.m_VkFunc.vkDestroyIndirectCommandsLayoutNV(m_VulkanState.m_Device, indirectCmdsLayout, nullptr);
			indirectCmdsLayout = nullptr;
		}
	}

	void VulkanIndirectDrawNV::ResetInput()
	{
		SPICES_PROFILE_ZONE;

		nMeshPack = 0;
		inputBuffer = nullptr;
		inputs.clear();
		preprocessBuffer = nullptr;
		preprocessSize = 0;
	}
}