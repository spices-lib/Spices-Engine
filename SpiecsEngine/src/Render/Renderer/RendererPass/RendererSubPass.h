/**
* @file RendererSubPass.h
* @brief The RendererSubPass Class Definitions.
* @author Spiecs.
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

namespace Spiecs
{
	/**
	* @brief This Class Combines some data relatived to subpass.
	* Usually as a member variable of RendererPass.
	*/
	class RendererSubPass
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] subPassName The name of subpass.
		* @param[in] index Subpass index of pass.
		*/
		RendererSubPass(
			const std::string& subPassName , 
			uint32_t           index
		) 
			: m_SubpassName(subPassName )
			, m_Index      (index       ) 
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~RendererSubPass() {};

		/**
		* @brief Add a Color Attachment to subpass.
		* @param[in] attachmentReference VkAttachmentReference.
		* @param[in] colorBlend VkPipelineColorBlendAttachmentState.
		*/
		void AddColorAttachmentReference( 
			const VkAttachmentReference&               attachmentReference,
			const VkPipelineColorBlendAttachmentState& colorBlend
		);

		/**
		* @brief Add a Depth Attachment to subpass.
		* @param[in] attachmentReference VkAttachmentReference.
		*/
		void AdDepthAttachmentReference(
			const VkAttachmentReference&               attachmentReference
		);

		/**
		* @brief Add a Input Attachment to subpass.
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
		* @brief Build First VkSubpassDependency.
		*/
		void BuildFirstSubPassDependency();

		/**
		* @brief Build VkSubpassDependency.
		* @param[in] index The index of subpass of pass.
		*/
		void BuildSubPassDependency(uint32_t index);

		/**
		* @brief Set Pipeline PushConstant.
		* @param[in] T The function of fill in VkPushConstantRange.
		*/
		template<typename T>
		void SetPushConstant(T fn);

		/**
		* @brief Get a VkSubpassDescription.
		* @return Returns the VkSubpassDescription.
		*/
		inline VkSubpassDescription& GetDescription() { return m_SubPassDescriptions; };

		/**
		* @brief Get a VkSubpassDependency.
		* @return Returns the VkSubpassDependency.
		*/
		inline VkSubpassDependency& GetDependency() { return m_SubPassDependency; };

		/**
		* @brief Get subpass name.
		* @return Returns the subpass name.
		*/
		const std::string& GetName() { return m_SubpassName; };

		/**
		* @brief Get isUsePushConstant.
		* @return Returns the isUsePushConstant.
		*/
		inline bool IsUsePushConstant() { return isUsePushConstant; };

		/**
		* @brief Get VkPushConstantRange.
		* @return Returns the VkPushConstantRange.
		*/
		inline VkPushConstantRange& GetPushConstant() { return m_PushConstantRange; };

		/**
		* @brief Get a buffer with index.
		* @param[in] i2 Index of buffer.
		* @return Returns the indexed buffer.
		*/
		std::shared_ptr<VulkanBuffer>& GetBuffers(const Int2& i2) { return m_Buffers[i2]; };

		/**
		* @brief Set Buffer data.
		* @param[in] i2 The Buffer Indfex.
		* @param[in] data The data copy from.
		* @param[in] size The copy size.
		* @param[in] offest The Copy offest.
		*/
		void SetBuffer(
			const Int2& i2                   , 
			void*       data                 , 
			uint64_t    size = VK_WHOLE_SIZE , 
			uint64_t    offest = 0
		);

		/**
		* @brief Get subpass index of pass.
		*/
		inline uint32_t GetIndex() { return m_Index; };

		/**
		* @brief Get VkPipelineColorBlendAttachmentState.
		* @return Returns VkPipelineColorBlendAttachmentState vector.
		*/
		inline std::vector<VkPipelineColorBlendAttachmentState>& GetColorBlend() { return m_ColorBlends; };

	private:

		/**
		* @brief Subpass Name.
		*/
		std::string m_SubpassName;

		/**
		* @brief Index of subpass in pass.
		*/
		uint32_t m_Index;

		/**
		* @brief VkSubpassDescription.
		*/
		VkSubpassDescription m_SubPassDescriptions{};

		/**
		* @brief VkSubpassDependency.
		*/
		VkSubpassDependency m_SubPassDependency{};

		/**
		* @brief Subpass used colorattachment.
		*/
		std::vector<VkAttachmentReference> m_ColorAttachmentReference;

		/**
		* @brief Subpass used depthattachment.
		*/
		std::vector<VkAttachmentReference> m_DepthAttachmentReference;

		/**
		* @brief Subpass used inputattachment.
		*/
		std::vector<VkAttachmentReference> m_InputAttachmentReference;

		/**
		* @brief Subpass used VkPipelineColorBlendAttachmentState.
		*/
		std::vector<VkPipelineColorBlendAttachmentState> m_ColorBlends;

		/**
		* @brief True if subpass use a PushConstant.
		*/
		bool isUsePushConstant = false;

		/**
		* @brief VkPushConstantRange.
		*/
		VkPushConstantRange m_PushConstantRange{};
		
		/**
		* @brief All Buffers used this subpass.
		*/
		std::unordered_map<Int2, std::shared_ptr<VulkanBuffer>> m_Buffers;
	};

	template<typename T>
	inline void RendererSubPass::SetPushConstant(T fn)
	{
		SPIECS_PROFILE_ZONE;

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