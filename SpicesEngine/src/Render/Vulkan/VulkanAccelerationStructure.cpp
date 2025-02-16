/**
* @file VulkanAccelerationStructure.cpp.
* @brief The VulkanAccelerationStructure Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanAccelerationStructure.h"

namespace Spices {

	VulkanAccelerationStructure::VulkanAccelerationStructure(
		VulkanState&                          vulkanState , 
		VkAccelerationStructureCreateInfoKHR& accel
	)
		: VulkanObject(vulkanState)
	    , m_Accel(nullptr)
	{
		SPICES_PROFILE_ZONE;

		vulkanState.m_VkFunc.vkCreateAccelerationStructureKHR(
			vulkanState.m_Device ,
			&accel               ,
			nullptr              ,
			&m_Accel
		);
	}

	VulkanAccelerationStructure::~VulkanAccelerationStructure()
	{
		SPICES_PROFILE_ZONE;

		m_VulkanState.m_VkFunc.vkDestroyAccelerationStructureKHR(m_VulkanState.m_Device, m_Accel, nullptr);
		m_Accel = nullptr;
	}

	VkDeviceAddress VulkanAccelerationStructure::GetACDeviceAddress() const
	{
		SPICES_PROFILE_ZONE;

		VkAccelerationStructureDeviceAddressInfoKHR     addressInfo{};
		addressInfo.sType                             = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
		addressInfo.accelerationStructure             = m_Accel;

		return m_VulkanState.m_VkFunc.vkGetAccelerationStructureDeviceAddressKHR(m_VulkanState.m_Device, &addressInfo);
	}
}