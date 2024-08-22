/**
* @file RendererSubPass.h
* @brief The RendererSubPass Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"

/******************************Vulkan Backend Header************************************************/
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "Render/Vulkan/VulkanRenderPass.h"
/***************************************************************************************************/

namespace Spices
{
	/**
	* @brief This Class Combines some data relative to sub pass.
	* Usually as a member variable of RendererPass.
	*/
	class RendererSubPass
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] subPassName The name of sub pass.
		* @param[in] index Sub pass index of pass.
		*/
		RendererSubPass(
			const std::string& subPassName , 
			uint32_t           index
		) 
			: m_SubpassName(subPassName )
			, m_Index      (index       ) 
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererSubPass() = default;

		/**
		* @brief Add a Color Attachment to sub pass.
		* @param[in] attachmentReference VkAttachmentReference.
		* @param[in] colorBlend VkPipelineColorBlendAttachmentState.
		*/
		void AddColorAttachmentReference( 
			const VkAttachmentReference&               attachmentReference,
			const VkPipelineColorBlendAttachmentState& colorBlend
		);

		/**
		* @brief Add a Depth Attachment to sub pass.
		* @param[in] attachmentReference VkAttachmentReference.
		*/
		void AddDepthAttachmentReference(
			const VkAttachmentReference&               attachmentReference
		);

		/**
		* @brief Add a Input Attachment to sub pass.
		* @param[in] attachmentReference VkAttachmentReference.
		*/
		void AddInputAttachmentReference(
			const VkAttachmentReference&               attachmentReference
		);

		/**
		* @brief Build VkSubpassDescription.
		*/
		void BuildSubPassDescription();
		
		/**
		* @brief Add First VkSubpassDependency.
		*/
		void AddFirstSubPassDependency();

		/**
		* @brief Build VkSubpassDependency.
		* @param[in] index The index of sub pass of pass.
		*/
		void BuildSubPassDependency(uint32_t index);

		/**
		* @brief Add VkSubpassDependency.
		* @param[in] srcsubpass The index of src sub pass of pass.
		* @param[in] dstsubpass The index of dst sub pass of pass.
		* @param[in] srcAccessMask VkAccessFlags
		* @param[in] dstAccessMask VkAccessFlags
		* @param[in] srcStageMask VkPipelineStageFlags
		* @param[in] dstStageMask VkPipelineStageFlags
		*/
		void AddSubPassDependency(
			uint32_t               srcsubpass    , 
			uint32_t               dstsubpass    ,
			VkAccessFlags          srcAccessMask ,
			VkAccessFlags          dstAccessMask ,
			VkPipelineStageFlags   srcStageMask  ,
			VkPipelineStageFlags   dstStageMask
		);

		/**
		* @brief Set Pipeline PushConstant.
		* @param[in] fn The function of fill in VkPushConstantRange.
		*/
		template<typename T>
		void SetPushConstant(T fn);

		/**
		* @brief Get a VkSubpassDescription.
		* @return Returns the VkSubpassDescription.
		*/
		inline VkSubpassDescription& GetDescription() { return m_SubPassDescriptions; }

		/**
		* @brief Get a VkSubpassDependency.
		* @return Returns the VkSubpassDependency.
		*/
		inline std::vector<VkSubpassDependency>& GetDependency() { return m_SubPassDependency; }

		/**
		* @brief Get sub pass name.
		* @return Returns the sub pass name.
		*/
		const std::string& GetName() { return m_SubpassName; }

		/**
		* @brief Get isUsePushConstant.
		* @return Returns the isUsePushConstant.
		*/
		bool IsUsePushConstant() const { return isUsePushConstant; }

		/**
		* @brief Get VkPushConstantRange.
		* @return Returns the VkPushConstantRange.
		*/
		VkPushConstantRange& GetPushConstant() { return m_PushConstantRange; }

		/**
		* @brief Get a buffer with index.
		* @param[in] i2 Index of buffer.
		* @return Returns the indexed buffer.
		*/
		std::shared_ptr<VulkanBuffer>& GetBuffers(const UInt2& i2) { return m_Buffers[i2]; }

		/**
		* @brief Set Buffer data.
		* @param[in] i2 The Buffer Index.
		* @param[in] data The data copy from.
		* @param[in] size The copy size.
		* @param[in] offset The Copy offset.
		*/
		void SetBuffer(
			const UInt2& i2                   , 
			void*        data                 , 
			uint64_t     size = VK_WHOLE_SIZE , 
			uint64_t     offset = 0
		);

		/**
		* @brief Get sub pass index of pass.
		*/
		uint32_t GetIndex() const { return m_Index; }

		/**
		* @brief Get VkPipelineColorBlendAttachmentState.
		* @return Returns VkPipelineColorBlendAttachmentState vector.
		*/
		std::vector<VkPipelineColorBlendAttachmentState>& GetColorBlend() { return m_ColorBlends; }

	private:

		/**
		* @brief Sub pass Name.
		*/
		std::string m_SubpassName;

		/**
		* @brief Index of sub pass in pass.
		*/
		uint32_t m_Index;

		/**
		* @brief VkSubpassDescription.
		*/
		VkSubpassDescription m_SubPassDescriptions{};

		/**
		* @brief VkSubpassDependency.
		*/
		std::vector<VkSubpassDependency> m_SubPassDependency{};

		/**
		* @brief Subpass used color attachment.
		*/
		std::vector<VkAttachmentReference> m_ColorAttachmentReference;

		/**
		* @brief Sub pass used depth attachment.
		*/
		std::vector<VkAttachmentReference> m_DepthAttachmentReference;

		/**
		* @brief Sub pass used input attachment.
		*/
		std::vector<VkAttachmentReference> m_InputAttachmentReference;

		/**
		* @brief Sub pass used VkPipelineColorBlendAttachmentState.
		*/
		std::vector<VkPipelineColorBlendAttachmentState> m_ColorBlends;

		/**
		* @brief True if sub pass use a PushConstant.
		*/
		bool isUsePushConstant = false;

		/**
		* @brief VkPushConstantRange.
		*/
		VkPushConstantRange m_PushConstantRange{};
		
		/**
		* @brief All Buffers used this sub pass.
		*/
		std::unordered_map<UInt2, std::shared_ptr<VulkanBuffer>> m_Buffers;
	};

	template<typename T>
	inline void RendererSubPass::SetPushConstant(T fn)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Set isUsePushConstant only if entrace this function.
		*/
		isUsePushConstant = true;

		/**
		* @brief Fill in VkPushConstantRange Data.
		* @param[in] m_PushConstantRange VkPushConstantRange.
		*/
		fn(m_PushConstantRange);
	}
}