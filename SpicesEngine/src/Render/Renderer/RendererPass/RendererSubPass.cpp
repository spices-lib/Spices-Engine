/**
* @file RendererSubPass.cpp.
* @brief The RendererSubPass Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RendererSubPass.h"

#include "Render/Renderer/DescriptorSetManager/DescriptorSetManager.h"

namespace Spices {

	void RendererSubPass::AddColorAttachmentReference(
		const VkAttachmentReference&               attachmentReference,
		const VkPipelineColorBlendAttachmentState& colorBlend
	)
	{
		SPICES_PROFILE_ZONE;
		
		m_ColorAttachmentReference.push_back(attachmentReference);
		m_ColorBlends.push_back(colorBlend);
	}

	void RendererSubPass::AddDepthAttachmentReference(
		const VkAttachmentReference&               attachmentReference
	)
	{
		SPICES_PROFILE_ZONE;

		m_DepthAttachmentReference.push_back(attachmentReference);
	}

	void RendererSubPass::AddInputAttachmentReference(
		const VkAttachmentReference&               attachmentReference
	)
	{
		SPICES_PROFILE_ZONE;

		m_InputAttachmentReference.push_back(attachmentReference);
	}

	void RendererSubPass::BuildSubPassDescription()
	{
		SPICES_PROFILE_ZONE;

		m_SubPassDescriptions.pipelineBindPoint         = VK_PIPELINE_BIND_POINT_GRAPHICS;
		m_SubPassDescriptions.colorAttachmentCount      = static_cast<uint32_t>(m_ColorAttachmentReference.size());
		m_SubPassDescriptions.pColorAttachments         = m_ColorAttachmentReference.data();
		m_SubPassDescriptions.pDepthStencilAttachment   = m_DepthAttachmentReference.empty() ? nullptr : m_DepthAttachmentReference.data();
		m_SubPassDescriptions.inputAttachmentCount      = static_cast<uint32_t>(m_InputAttachmentReference.size());
		m_SubPassDescriptions.pInputAttachments         = m_InputAttachmentReference.data();
	}

	void RendererSubPass::AddFirstSubPassDependency()
	{
		SPICES_PROFILE_ZONE;

		VkSubpassDependency                   dependency{};
		dependency.srcSubpass               = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass               = 0;
		dependency.srcStageMask             = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask             = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask            = 0;
		dependency.dstAccessMask            = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags          = 0;

		m_SubPassDependency.push_back(std::move(dependency));
	}

	void RendererSubPass::BuildSubPassDependency(uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		VkSubpassDependency                   dependency{};
		dependency.srcSubpass               = index - 1;
		dependency.dstSubpass               = index;
		dependency.srcStageMask             = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask             = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependency.srcAccessMask            = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask            = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		dependency.dependencyFlags          = VK_DEPENDENCY_BY_REGION_BIT;

		m_SubPassDependency.push_back(std::move(dependency));
	}

	void RendererSubPass::AddSubPassDependency(
		uint32_t               srcsubpass    ,
		uint32_t               dstsubpass    ,
		VkAccessFlags          srcAccessMask ,
		VkAccessFlags          dstAccessMask ,
		VkPipelineStageFlags   srcStageMask  ,
		VkPipelineStageFlags   dstStageMask
	)
	{
		SPICES_PROFILE_ZONE;

		VkSubpassDependency                   dependency{};
		dependency.srcSubpass               = srcsubpass;
		dependency.dstSubpass               = dstsubpass;
		dependency.srcStageMask             = srcStageMask;
		dependency.dstStageMask             = dstStageMask;
		dependency.srcAccessMask            = srcAccessMask;
		dependency.dstAccessMask            = dstAccessMask;
		dependency.dependencyFlags          = VK_DEPENDENCY_BY_REGION_BIT;

		m_SubPassDependency.push_back(std::move(dependency));
	}

	void RendererSubPass::SetBuffer(const UInt2& i2, void* data, uint64_t size, uint64_t offset)
	{
		SPICES_PROFILE_ZONE;

		m_Buffers[i2]->WriteToBuffer(data, size, offset);
		m_Buffers[i2]->Flush();
	}
}