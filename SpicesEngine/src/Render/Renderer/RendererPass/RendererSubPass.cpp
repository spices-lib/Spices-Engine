/**
* @file RendererSubPass.cpp.
* @brief The RendererSubPass Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RendererSubPass.h"

#include "Render/Renderer/DescriptorSetManager/DescriptorSetManager.h"
#include "Core/Library/ContainerLibrary.h"

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

		m_SubPassDependency.srcSubpass      = VK_SUBPASS_EXTERNAL;
		m_SubPassDependency.dstSubpass      = 0;
		m_SubPassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		m_SubPassDependency.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		m_SubPassDependency.srcAccessMask   = 0;
		m_SubPassDependency.dstAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		m_SubPassDependency.dependencyFlags = 0;
	}

	void RendererSubPass::BuildSubPassDependency(uint32_t index)
	{
		SPICES_PROFILE_ZONE;

		m_SubPassDependency.srcSubpass      = index - 1;
		m_SubPassDependency.dstSubpass      = index;
		m_SubPassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		m_SubPassDependency.dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		m_SubPassDependency.srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		m_SubPassDependency.dstAccessMask   = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		m_SubPassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
	}

	void RendererSubPass::SetBuffer(const UInt2& i2, void* data, uint64_t size, uint64_t offset)
	{
		SPICES_PROFILE_ZONE;

		m_Buffers[i2]->WriteToBuffer(data, size, offset);
		m_Buffers[i2]->Flush();
	}
}