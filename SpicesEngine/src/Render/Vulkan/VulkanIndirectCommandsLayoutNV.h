/**
* @file VulkanIndirectCommandsLayoutNV.h.
* @brief The VulkanIndirectCommandsLayoutNV Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief VulkanIndirectCommandsLayoutNV Class.
	* This class is just a wrapper of VkIndirectCommandsLayoutNV.
	*/
	class VulkanIndirectCommandsLayoutNV : VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create vkInstance and vkSurface.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] info CreateInfo.
		*/
		VulkanIndirectCommandsLayoutNV(
			VulkanState&                          vulkanState ,
			VkIndirectCommandsLayoutCreateInfoNV& info
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanIndirectCommandsLayoutNV() override;

		/**
		* @brief Get this VkIndirectCommandsLayoutNV instance.
		* @reutrn Returns VkIndirectCommandsLayoutNV instance.
		*/
		VkIndirectCommandsLayoutNV& Get() { return m_IndirectCmdsLayout; }

	private:


		/**
		* @brief This VkIndirectCommandsLayoutNV instance.
		*/
		VkIndirectCommandsLayoutNV m_IndirectCmdsLayout;
	};
}