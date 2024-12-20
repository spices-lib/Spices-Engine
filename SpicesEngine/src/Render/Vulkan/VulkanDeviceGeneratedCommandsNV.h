/**
* @file VulkanDeviceGeneratedCommandsNV.h.
* @brief The VulkanDeviceGeneratedCommandsNV Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanBuffer.h"
#include "VulkanIndirectCommandsLayoutNV.h"
#include "VulkanPipeline.h"

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
		virtual ~VulkanDeviceGeneratedCommandsNV() override = default;

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
		* @param[in] strides InputStrides.
		*/
		void SetInputStride(const std::vector<uint32_t>& strides) { m_InputStrides = strides; }

		/**
		* @brief Get Strides.
		* @return Returns Strides.
		*/
		const uint32_t GetStrides() const { return m_Strides; }

		/**
		* @brief Set Strides.
		* @param[in] stride Strides.
		*/
		void SetStrides(uint32_t stride) { m_Strides = stride; }

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
		*/
		void CreatePreprocessBuffer();

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
		* @brief Set Layout Tokens.
		* @param[in] layouts Layout Tokens.
		*/
		void SetLayoutTokens(const std::vector<VkIndirectCommandsLayoutTokenNV>& layouts) { m_LayoutTokens = layouts; }

		/**
		* @brief Get DGC pipeline references.
		* @return Returns DGC pipeline references.
		*/
		std::vector<VkPipeline>& GetPipelineRef() { return m_PipelineRef; }

		/**
		* @brief Build Raytracing Pipeline.
		* @param[in] pipelineName pipeline's name.
		* @param[in] materialName material's name.
		* @param[in] config pipeline config.
		*/
		void CreateMeshPipeline(const std::string& pipelineName, const std::string& materialName, PipelineConfigInfo& config);

		/**
		* @brief Get NSequence.
		* @return Returns NSequence.
		*/
		uint32_t GetSequences() const { return m_NSequence; }

		/**
		* @brief Get Command Layout.
		* @return Returns Command Layout.
		*/
		std::shared_ptr<VulkanIndirectCommandsLayoutNV> GetCommandLayout() const { return m_IndirectCmdsLayout; }

		/**
		* @brief Set Command Layout.
		* @param[in] layout VkIndirectCommandsLayoutNV.
		*/
		void SetCommandLayout(std::shared_ptr<VulkanIndirectCommandsLayoutNV> layout) { m_IndirectCmdsLayout = layout; }

		/**
		* @brief Preprocess with Indirect Command Buffer.
		* @param[in] cmdBuffer VkCommandBuffer.
		*/
		void PreprocessDGC(const VkCommandBuffer& cmdBuffer) const;

		/**
		* @brief Execute Commands in Indirect Command Buffer.
		* @param[in] cmdBuffer VkCommandBuffer.
		*/
		void ExecuteDGC(const VkCommandBuffer& cmdBuffer) const;

	private:
		
		/**
		* @brief DrawCall elements Input Strides. 
		*/
		std::vector<uint32_t> m_InputStrides;

		/**
		* @brief DrawCall total Input Strides. 
		*/
		uint32_t m_Strides;

		/**
		* @brief VkIndirectCommandsLayoutNV. 
		*/
		std::shared_ptr<VulkanIndirectCommandsLayoutNV> m_IndirectCmdsLayout;

		/**
		* @brief VkIndirectCommandsLayoutTokenNV. 
		*/
		std::vector<VkIndirectCommandsLayoutTokenNV> m_LayoutTokens;

		/**
		* @brief Draw count. 
		*/
		uint32_t m_NSequence;

		/**
		* @brief DrawCall Input Buffer. 
		*/
		std::shared_ptr<VulkanBuffer> m_InputBuffer;

		/**
		* @brief VkIndirectCommandsStreamNV. 
		*/
		std::vector<VkIndirectCommandsStreamNV> m_InputStreams;

		/**
		* @brief PreprocessBuffer. 
		*/
		std::shared_ptr<VulkanBuffer> m_PreprocessBuffer;

		/**
		* @brief PreprocessBuffer bytes. 
		*/
		uint32_t m_PreprocessSize;

		/**
		* @brief DGC pipeline. 
		*/
		std::shared_ptr<VulkanPipeline> m_Pipeline;

		/**
		* @brief DrawCall referenced pipeline. 
		*/
		std::vector<VkPipeline> m_PipelineRef;
	};
}
