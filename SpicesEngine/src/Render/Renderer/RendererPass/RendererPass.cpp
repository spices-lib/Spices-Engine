#include "Pchheader.h"
#include "RendererPass.h"
#include "Render/Vulkan/VulkanRenderBackend.h"
#include "Render/Renderer/DescriptorSetManager/DescriptorSetManager.h"

namespace Spices {

	RendererPass::~RendererPass()
	{
		SPICES_PROFILE_ZONE;
		
		m_SubPasses.for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subpass) {
			const String2 s2(m_PassName, name);
			DescriptorSetManager::UnLoad(s2);
			return false;
		});
	}

	std::shared_ptr<RendererSubPass> RendererPass::AddSubPass(const std::string& subPassName, uint32_t index)
	{
		SPICES_PROFILE_ZONE;
		
		if (m_SubPasses.has_key(subPassName))
		{
			std::stringstream ss;
			ss << "RendererPass: " << m_PassName << ": SubPass: " << subPassName << " already added.";

			SPICES_CORE_WARN(ss.str());
			return nullptr;
		}

		auto ptr = std::make_shared<RendererSubPass>(subPassName, index);
		m_SubPasses.push_back(subPassName, ptr);
		return ptr;
	}

	uint32_t RendererPass::AddAttachment(
		const std::string&             attachmentName     , 
		const VkAttachmentDescription& description        , 
		uint32_t                       layers             ,
		const VkClearValue&            clearValue 
	)
	{
		SPICES_PROFILE_ZONE;
		
		if (m_AttachmentDescriptions.has_key(attachmentName))
		{
			uint32_t index = 0;
			m_AttachmentDescriptions.for_each([&](const auto& K, const auto& V) {
				if (K == attachmentName)
				{
					return true;
				}
				else
				{
					index++;
					return false;
				}
			});
			return index;
		}

		if (attachmentName == "SwapChainImage")
		{
			m_IsSwapChainImageInUse = true;
		}

		m_MaxLayers = glm::max(m_MaxLayers, layers);
		m_ClearValues.push_back(clearValue);
		m_AttachmentDescriptions.push_back(attachmentName, description);

		return static_cast<uint32_t>(m_AttachmentDescriptions.size() - 1);
	}

	uint32_t RendererPass::AddAttachment(
		const std::string&             attachmentName    , 
		const VkAttachmentDescription& description       ,
		const VkClearValue&            clearValue        ,
		uint32_t                       layers            ,
		const VkImageView&             view
	)
	{
		SPICES_PROFILE_ZONE;
		
		if (m_AttachmentDescriptions.has_key(attachmentName))
		{
			uint32_t index = 0;
			m_AttachmentDescriptions.for_each([&](const auto& K, const auto& V) {
				if (K == attachmentName)
				{
					return true;
				}
				else
				{
					index++;
					return false;
				}
			});
			return index;
		}

		if (attachmentName == "SwapChainImage")
		{
			m_IsSwapChainImageInUse = true;
		}

		m_MaxLayers = glm::max(m_MaxLayers, layers);
		m_ClearValues.push_back(clearValue);
		m_AttachmentDescriptions.push_back(attachmentName, description);
		m_ImageViews.push_back(view);

		return static_cast<uint32_t>(m_AttachmentDescriptions.size() - 1);
	}

	void RendererPass::BuildRendererPass()
	{
		SPICES_PROFILE_ZONE;
		
		std::vector<VkAttachmentDescription> attachmentDescription;	
		m_AttachmentDescriptions.for_each([&](const std::string& name, const VkAttachmentDescription& description) {
			attachmentDescription.push_back(description);
			return false;
		});

		std::vector<VkSubpassDescription> subPassDescription;
		m_SubPasses.for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subpass) {
			subPassDescription.push_back(subpass->GetDescription());
			return false;
		});

		std::vector<VkSubpassDependency> subPassDependency;
		(*m_SubPasses.first())->AddFirstSubPassDependency();
		m_SubPasses.for_each([&](const std::string& name, const std::shared_ptr<RendererSubPass>& subpass) {
			subPassDependency.insert(subPassDependency.end(), subpass->GetDependency().begin(), subpass->GetDependency().end());
			return false;
		});

		VkSubpassDependency                               outDependency{};
		outDependency.srcSubpass                        = static_cast<uint32_t>(m_SubPasses.size() - 1);
		outDependency.dstSubpass                        = VK_SUBPASS_EXTERNAL;
		outDependency.srcStageMask                      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		outDependency.dstStageMask                      = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		outDependency.srcAccessMask                     = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		outDependency.dstAccessMask                     = VK_ACCESS_MEMORY_READ_BIT;
		outDependency.dependencyFlags                   = VK_DEPENDENCY_BY_REGION_BIT;

		subPassDependency.push_back(outDependency);

		VkRenderPassCreateInfo                            renderPassInfo{};
		renderPassInfo.sType                            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount                  = static_cast<uint32_t>(attachmentDescription.size());
		renderPassInfo.pAttachments                     = attachmentDescription.data();
		renderPassInfo.subpassCount                     = static_cast<uint32_t>(subPassDescription.size());
		renderPassInfo.pSubpasses                       = subPassDescription.data();
		renderPassInfo.dependencyCount                  = static_cast<uint32_t>(subPassDependency.size());
		renderPassInfo.pDependencies                    = subPassDependency.data();

		if (m_AttachmentDescriptions.size() != m_ImageViews.size() + 1 && m_AttachmentDescriptions.size() != m_ImageViews.size())
		{
			std::stringstream ss;
			ss << m_PassName << ": RendererPss Create Failed: Not enough imageview for attachment.";

			SPICES_CORE_ERROR(ss.str());
		}

		m_RenderPass = std::make_unique<VulkanRenderPass>(
			VulkanRenderBackend::GetState(),
			m_PassName,
			m_Device, 
			renderPassInfo, 
			m_ImageViews,
			m_MaxLayers,
			m_IsSwapChainImageInUse
		);
	}
}