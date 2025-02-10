/**
* @file VulkanAccelerationStructure.h.
* @brief The VulkanAccelerationStructure Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief This Class is a Wrapper of VkAccelerationStructure.
	*/
	class VulkanAccelerationStructure : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkBuffer.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] accel VkAccelerationStructureCreateInfoKHR.
		*/
		VulkanAccelerationStructure(
			VulkanState&                          vulkanState ,
			VkAccelerationStructureCreateInfoKHR& accel
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanAccelerationStructure() override;

		/**
		* @brief Get AC Buffer Address.
		* @return Returns AC Buffer Address.
		*/
		VkDeviceAddress GetACDeviceAddress() const;

		/**
		* @brief Get this wrapped VkAccelerationStructureKHR.
		* @return Returns VkAccelerationStructureKHR.
		*/
		VkAccelerationStructureKHR& Get() { return m_Accel; }

	private:

		/**
		* @brief This wrapped VkAccelerationStructureKHR.
		*/
		VkAccelerationStructureKHR m_Accel;
	};
}