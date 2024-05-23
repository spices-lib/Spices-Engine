#include "Pchheader.h"
#include "RendererSubPass.h"
#include "Render/Renderer/DescriptorSetManager/DescriptorSetManager.h"
#include "Core/Library/ContainerLibrary.h"

namespace Spiecs {

	void RendererSubPass::AddColorAttachmentReference(
		const VkAttachmentReference&               attachmentReference,
		const VkPipelineColorBlendAttachmentState& colorBlend
	)
	{
		m_ColorAttachmentReference.push_back(attachmentReference);
		m_ColorBlends.push_back(colorBlend);
	}

	void RendererSubPass::AdDepthAttachmentReference(
		const VkAttachmentReference&               attachmentReference
	)
	{
		m_DepthAttachmentReference.push_back(attachmentReference);
	}

	void RendererSubPass::AddInputAttachmentReference(
		const VkAttachmentReference&               attachmentReference
	)
	{
		m_InputAttachmentReference.push_back(attachmentReference);
	}

	void RendererSubPass::BuildSubPassDescription()
	{
		m_SubPassDescriptions.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;

		m_SubPassDescriptions.colorAttachmentCount    = m_ColorAttachmentReference.size();
		m_SubPassDescriptions.pColorAttachments       = m_ColorAttachmentReference.data();

		m_SubPassDescriptions.pDepthStencilAttachment = m_DepthAttachmentReference.size() != 0 ? m_DepthAttachmentReference.data() : nullptr;

		m_SubPassDescriptions.inputAttachmentCount    = m_InputAttachmentReference.size();
		m_SubPassDescriptions.pInputAttachments       = m_InputAttachmentReference.data();
	}

	void RendererSubPass::BuildFirstSubPassDependency()
	{
		m_SubPassDependency.srcSubpass      = VK_SUBPASS_EXTERNAL;
		m_SubPassDependency.dstSubpass      = 0;
		m_SubPassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
		m_SubPassDependency.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;;
		m_SubPassDependency.srcAccessMask   = 0;
		m_SubPassDependency.dstAccessMask   = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		m_SubPassDependency.dependencyFlags = 0;
	}

	void RendererSubPass::BuildSubPassDependency(uint32_t index)
	{
		m_SubPassDependency.srcSubpass      = index - 1;
		m_SubPassDependency.dstSubpass      = index;
		m_SubPassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		m_SubPassDependency.dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		m_SubPassDependency.srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		m_SubPassDependency.dstAccessMask   = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		m_SubPassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
	}

	void RendererSubPass::AddPipeline(const std::string& name, std::shared_ptr<VulkanPipeline> pipeline)
	{
		m_Pipelines[name] = pipeline;
	}

	void RendererSubPass::SetBuffer(const Int2& i2, void* data)
	{
		m_Buffers[i2]->WriteToBuffer(data);
		m_Buffers[i2]->Flush();
	}
}