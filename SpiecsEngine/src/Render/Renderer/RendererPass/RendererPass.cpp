#include "Pchheader.h"
#include "RendererPass.h"

namespace Spiecs {

	std::shared_ptr<RendererSubPass> RendererPass::AddSubPass(const std::string& subPassName)
	{
		if (m_SubPasses.has_key(subPassName))
		{
			std::stringstream ss;
			ss << "RendererPass: " << m_PassName << ": SubPass: " << subPassName << " already added.";

			SPIECS_CORE_WARN(ss.str());
			return nullptr;
		}

		auto ptr = std::make_shared<RendererSubPass>(subPassName);
		m_SubPasses.push_back(subPassName, ptr);
		return ptr;
	}

	void RendererPass::AddAttachmentDescription(
		const std::string&             attachmnetName , 
		const VkAttachmentDescription& description    ,
		const VkClearValue&            clearValue
	)
	{
		if (m_AttachmentDescriptions.has_key(attachmnetName))
		{
			std::stringstream ss;
			ss << "RendererPass: " << m_PassName << ": Attachment: " << attachmnetName << " already added.";

			SPIECS_CORE_WARN(ss.str());
			return;
		}

		m_ClearValues.push_back(clearValue);
		m_AttachmentDescriptions.push_back(attachmnetName, description);
	}

	void RendererPass::BuildRendererPass()
	{
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(m_Attachments.size());
		renderPassInfo.pAttachments = m_Attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VK_CHECK(vkCreateRenderPass(m_VulkanState.m_Device, &renderPassInfo, nullptr, &m_RenderPass));

	}
}