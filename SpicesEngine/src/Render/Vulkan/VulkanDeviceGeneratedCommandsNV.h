/**
* @file VulkanDeviceGeneratedCommandsNV.h.
* @brief The VulkanDeviceGeneratedCommandsNV Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanBuffer.h"

namespace Spices {

	/**
	* @brief VulkanDeviceGeneratedCommandsNV Class.
	* This class defines the VulkanDeviceGeneratedCommandsNV behaves.
	*/
	class VulkanDeviceGeneratedCommandsNV : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* Create VkCommandPool.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanDeviceGeneratedCommandsNV(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanDeviceGeneratedCommandsNV() override;

		/**
		* @brief Reset this CommandsLayout.
		*/
		void ResetCommandsLayout();

		/**
		* @brief Reset this Input.
		*/
		void ResetInput();

		/**
		* @brief Add a stride to m_InputStrides.
		* @param[in] stride .
		*/
		void AddInputStride(uint32_t stride);

		/**
		* @brief Get InputStrides.
		* @return Returns InputStrides.
		*/
		const std::vector<uint32_t>& GetInputStrides() const { return m_InputStrides; }

		/**
		* @brief Set InputStrides.
		* @param[in] streams InputStreams.
		*/
		void SetInputStreams(const std::vector<VkIndirectCommandsStreamNV>& streams) { m_InputStreams = streams; }

		/**
		* @brief Create Input Buffer.
		* @param[in] size Buffer Size.
		* @return Returns Input Buffer.
		*/
		std::shared_ptr<VulkanBuffer> CreateInputBuffer(uint32_t size);

		/**
		* @brief Create Process Buffer.
		* @param[in] size Buffer Size.
		* @return Returns Input Buffer.
		*/
		std::shared_ptr<VulkanBuffer> CreatePreprocessBuffer(uint32_t size);

		/**
		* @brief Set Preprocess Size.
		*/
		void SetPreprocessSize(uint32_t size) { m_PreprocessSize = size; }

		/**
		* @brief Set SequenceCount.
		* @param[in] nSequences SequenceCount.
		*/
		void SetSequenceCount(uint32_t nSequences) { m_NSequence = nSequences; }

		/**
		* @brief Build CommandLayout.
		* @param[in] inputInfos .
		*/
		void BuildCommandLayout(const std::vector<VkIndirectCommandsLayoutTokenNV>& inputInfos);

		/**
		* @brief Get Layout Tokens.
		* @return Returns Layout Tokens.
		*/
		const std::vector<VkIndirectCommandsLayoutTokenNV>& GetLayoutTokens() const { return m_LayoutTokens; }

		/**
		* @brief Get Command Layout.
		* @return Returns Command Layout.
		*/
		VkIndirectCommandsLayoutNV GetCommandLayout() const { return m_IndirectCmdsLayout; }

		/**
		* @brief Preprocess with Indirect Command Buffer.
		* @param[in] cmdBuffer VkCommandBuffer.
		* @param[in] pipeline VkPipeline.
		*/
		void PreprocessDGC(const VkCommandBuffer& cmdBuffer, VkPipeline pipeline) const;

		/**
		* @brief Execute Commands in Indirect Command Buffer.
		* @param[in] cmdBuffer VkCommandBuffer.
		* @param[in] pipeline VkPipeline.
		*/
		void ExecuteDGC(const VkCommandBuffer& cmdBuffer, VkPipeline pipeline) const;

	private:

		std::vector<uint32_t>                        m_InputStrides;
		uint32_t                                     m_Strides;
		VkIndirectCommandsLayoutNV                   m_IndirectCmdsLayout;
		std::vector<VkIndirectCommandsLayoutTokenNV> m_LayoutTokens;

		uint32_t                                     m_NSequence;
		std::shared_ptr<VulkanBuffer>                m_InputBuffer;
		std::vector<VkIndirectCommandsStreamNV>      m_InputStreams;
		std::shared_ptr<VulkanBuffer>                m_PreprocessBuffer;
		uint32_t                                     m_PreprocessSize;
	};
}
