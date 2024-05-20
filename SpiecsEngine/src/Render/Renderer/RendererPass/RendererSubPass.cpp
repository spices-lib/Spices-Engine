#include "Pchheader.h"
#include "RendererSubPass.h"

namespace Spiecs {

	void RendererSubPass::AddColorAttachmentReference(
		const std::string&           attachmentName      , 
		VkAttachmentReference&       attachmentReference
	)
	{
		if (m_ColorAttachmentReference.has_key(attachmentName)) return;

		attachmentReference.attachment = m_ColorAttachmentReference.size();
		m_ColorAttachmentReference.push_back(attachmentName, attachmentReference);
	}

	void RendererSubPass::AdDepthAttachmentReference(
		const std::string&           attachmentName      , 
		VkAttachmentReference&       attachmentReference
	)
	{
		if (m_DepthAttachmentReference.has_key(attachmentName)) return;

		attachmentReference.attachment = m_DepthAttachmentReference.size();
		m_DepthAttachmentReference.push_back(attachmentName, attachmentReference);
	}

	void RendererSubPass::AddInputAttachmentReference(
		const std::string&           attachmentName      , 
		VkAttachmentReference&       attachmentReference
	)
	{
		if (m_InputAttachmentReference.has_key(attachmentName)) return;

		attachmentReference.attachment = m_InputAttachmentReference.size();
		m_InputAttachmentReference.push_back(attachmentName, attachmentReference);
	}

	void RendererSubPass::BuildSubPassDescription()
	{
		std::vector<VkAttachmentReference> color;
		m_ColorAttachmentReference.for_each([&](const std::string& name, const VkAttachmentReference& ref) {
			color.push_back(ref);
		});

		std::vector<VkAttachmentReference> depth;
		m_DepthAttachmentReference.for_each([&](const std::string& name, const VkAttachmentReference& ref) {
			depth.push_back(ref);
		});

		std::vector<VkAttachmentReference> input;
		m_InputAttachmentReference.for_each([&](const std::string& name, const VkAttachmentReference& ref) {
			input.push_back(ref);
		});

		m_SubPassDescriptions.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;

		m_SubPassDescriptions.colorAttachmentCount    = color.size();
		m_SubPassDescriptions.pColorAttachments       = color.data();

		m_SubPassDescriptions.pDepthStencilAttachment = depth.size() != 0 ? depth.data() : nullptr;

		m_SubPassDescriptions.inputAttachmentCount    = input.size();
		m_SubPassDescriptions.pInputAttachments       = input.data();
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
}