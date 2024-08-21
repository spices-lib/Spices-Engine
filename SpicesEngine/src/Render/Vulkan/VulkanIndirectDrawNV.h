/**
* @file VulkanIndirectDrawNV.h.
* @brief The VulkanIndirectDrawNV Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanBuffer.h"

namespace Spices {

	/**
	* @brief VulkanIndirectDrawNV Class.
	* This class defines the VulkanIndirectDrawNV behaves.
	*/
	class VulkanIndirectDrawNV : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkCommandPool.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanIndirectDrawNV(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanIndirectDrawNV() override;

		/**
		* @brief Reset this CommandsLayout.
		*/
		void ResetCommandsLayout();

		/**
		* @brief Reset this Input.
		*/
		void ResetInput();

	private:

		std::vector<uint32_t>                   m_InputStrides;
		uint32_t                                m_Strides;
		VkIndirectCommandsLayoutNV              m_IndirectCmdsLayout;

		uint32_t                                m_NSequence;
		std::unique_ptr<VulkanBuffer>           m_InputBuffer;
		std::vector<VkIndirectCommandsStreamNV> m_Inputs;
		std::unique_ptr<VulkanBuffer>           m_PreprocessBuffer;
		uint32_t                                m_PreprocessSize;
	};
}
