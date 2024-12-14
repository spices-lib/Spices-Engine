/**
* @file VulkanIndirectCommmandsLayoutNV.h.
* @brief The VulkanIndirectCommmandsLayoutNV Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief VulkanIndirectCommmandsLayoutNV Class.
	* This class is just a wrapper of VkIndirectCommandsLayoutNV.
	*/
	class VulkanIndirectCommmandsLayoutNV : VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create vkInstance and vkSurface.
		* @param[in] vulkanState The global VulkanState.
		* @param[in] info CreateInfo.
		*/
		VulkanIndirectCommmandsLayoutNV(
			VulkanState&                          vulkanState ,
			VkIndirectCommandsLayoutCreateInfoNV& info
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanIndirectCommmandsLayoutNV() override;

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