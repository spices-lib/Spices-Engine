#include "Pchheader.h"
#include "RendererSubPass.h"

namespace Spiecs {

	void RendererSubPass::AddColorAttachmentReference(
		const std::string&           attachmentName      , 
		VkAttachmentReference&       attachmentReference
	)
	{
		if (m_ColorAttachmentReference.find(attachmentName) != m_ColorAttachmentReference.end()) return;

		attachmentReference.attachment = m_ColorAttachmentReference.size();
		m_ColorAttachmentReference[attachmentName] = attachmentReference;
	}

	void RendererSubPass::AdDepthAttachmentReference(
		const std::string&           attachmentName      , 
		VkAttachmentReference&       attachmentReference
	)
	{
		if (m_DepthAttachmentReference.find(attachmentName) != m_DepthAttachmentReference.end()) return;

		attachmentReference.attachment = m_DepthAttachmentReference.size();
		m_DepthAttachmentReference[attachmentName] = attachmentReference;
	}

	void RendererSubPass::AddInputAttachmentReference(
		const std::string&           attachmentName      , 
		VkAttachmentReference&       attachmentReference
	)
	{
		if (m_InputAttachmentReference.find(attachmentName) != m_InputAttachmentReference.end()) return;

		attachmentReference.attachment = m_InputAttachmentReference.size();
		m_InputAttachmentReference[attachmentName] = attachmentReference;
	}

	void RendererSubPass::BuildSubPassDescription()
	{
		std::vector<VkAttachmentReference> color;
		for (auto& pair : m_ColorAttachmentReference)
		{
			color.push_back(pair.second);
		}

		std::vector<VkAttachmentReference> depth;
		for (auto& pair : m_DepthAttachmentReference)
		{
			depth.push_back(pair.second);
		}

		std::vector<VkAttachmentReference> input;
		for (auto& pair : m_InputAttachmentReference)
		{
			input.push_back(pair.second);
		}

		m_SubPassDescriptions.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;

		m_SubPassDescriptions.colorAttachmentCount    = color.size();
		m_SubPassDescriptions.pColorAttachments       = color.data();

		m_SubPassDescriptions.pDepthStencilAttachment = depth.size() != 0 ? depth.data() : nullptr;

		m_SubPassDescriptions.inputAttachmentCount    = input.size();
		m_SubPassDescriptions.pInputAttachments       = input.data();
	}
}