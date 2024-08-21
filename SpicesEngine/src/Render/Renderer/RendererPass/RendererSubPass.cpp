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

	void RendererSubPass::BuildFirstSubPassDependency()
	{
		SPICES_PROFILE_ZONE;

		m_SubPassDependency[0].srcSubpass      = VK_SUBPASS_EXTERNAL;
		m_SubPassDependency[0].dstSubpass      = 0;
		m_SubPassDependency[0].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		m_SubPassDependency[0].dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		m_SubPassDependency[0].srcAccessMask   = 0;
		m_SubPassDependency[0].dstAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		m_SubPassDependency[0].dependencyFlags = 0;
	}

	void RendererSubPass::BuildSubPassDependency(uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		m_SubPassDependency.resize(3);

		m_SubPassDependency[0].srcSubpass      = index - 1;
		m_SubPassDependency[0].dstSubpass      = index;
		m_SubPassDependency[0].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		m_SubPassDependency[0].dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		m_SubPassDependency[0].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		m_SubPassDependency[0].dstAccessMask   = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		m_SubPassDependency[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		m_SubPassDependency[1].srcSubpass      = index;
		m_SubPassDependency[1].dstSubpass      = index;
		m_SubPassDependency[1].srcStageMask    = VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV;
		m_SubPassDependency[1].dstStageMask    = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		m_SubPassDependency[1].srcAccessMask   = VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV;
		m_SubPassDependency[1].dstAccessMask   = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
		m_SubPassDependency[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		m_SubPassDependency[2].srcSubpass      = index;
		m_SubPassDependency[2].dstSubpass      = index;
		m_SubPassDependency[2].srcStageMask    = VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		m_SubPassDependency[2].dstStageMask    = VK_PIPELINE_STAGE_NONE;
		m_SubPassDependency[2].srcAccessMask   = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
		m_SubPassDependency[2].dstAccessMask   = VK_ACCESS_NONE;
		m_SubPassDependency[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
	}

	void RendererSubPass::SetBuffer(const UInt2& i2, void* data, uint64_t size, uint64_t offset)
	{
		SPICES_PROFILE_ZONE;

		m_Buffers[i2]->WriteToBuffer(data, size, offset);
		m_Buffers[i2]->Flush();
	}
}