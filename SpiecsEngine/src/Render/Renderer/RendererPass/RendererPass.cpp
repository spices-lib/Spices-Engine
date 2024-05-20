#include "Pchheader.h"
#include "RendererPass.h"
#include "Render/Vulkan/VulkanRenderBackend.h"

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

	void RendererPass::AddAttachment(
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

		if (attachmnetName == "SwapChainImage")
		{
			m_IsSwapChainImageInUse = true;
		}

		m_ClearValues.push_back(clearValue);
		m_AttachmentDescriptions.push_back(attachmnetName, description);
	}

	void RendererPass::AddAttachment(
		const std::string&             attachmnetName , 
		const VkAttachmentDescription& description    ,
		const VkClearValue&            clearValue     ,
		VkImageView&                   view
	)
	{
		AddAttachment(attachmnetName, description, clearValue);
		m_ImageViews.push_back(view);
	}

	void RendererPass::BuildRendererPass()
	{
		std::vector<VkAttachmentDescription> attachmentDescription;
		m_AttachmentDescriptions.for_each([&](const std::string& name, const VkAttachmentDescription& description) {
			attachmentDescription.push_back(description);
		});

		std::vector<VkSubpassDescription> subPassDescription;
		m_SubPasses.for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subpass) {
			subPassDescription.push_back(subpass->GetDescription());
		});

		std::vector<VkSubpassDependency> subPassDepecdency;
		m_SubPasses.first()->BuildFirstSubPassDependency();
		m_SubPasses.for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subpass) {
			subPassDepecdency.push_back(subpass->GetDependency());
		});

		VkSubpassDependency outDependency{};
		outDependency.srcSubpass      = m_SubPasses.size();
		outDependency.dstSubpass      = VK_SUBPASS_EXTERNAL;
		outDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		outDependency.dstStageMask    = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		outDependency.srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		outDependency.dstAccessMask   = VK_ACCESS_MEMORY_READ_BIT;
		outDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		subPassDepecdency.push_back(outDependency);

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescription.size());
		renderPassInfo.pAttachments    = attachmentDescription.data();
		renderPassInfo.subpassCount    = subPassDescription.size();
		renderPassInfo.pSubpasses      = subPassDescription.data();
		renderPassInfo.dependencyCount = subPassDepecdency.size();
		renderPassInfo.pDependencies   = subPassDepecdency.data();

		if (m_AttachmentDescriptions.size() != m_ImageViews.size() + 1 && m_AttachmentDescriptions.size() != m_ImageViews.size())
		{
			std::stringstream ss;
			ss << m_PassName << ": RendererPss Create Failed: Not enough imageview for attachment.";

			SPIECS_CORE_ERROR(ss.str());
		}

		m_RenderPass = std::make_unique<VulkanRenderPass>(
			VulkanRenderBackend::GetState(),
			m_Device, 
			renderPassInfo, 
			m_ImageViews,
			m_IsSwapChainImageInUse
		);
	}
}