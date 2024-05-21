/**
* @file SceneComposeRenderer.cpp.
* @brief The SceneComposeRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SceneComposeRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	SceneComposeRenderer::SceneComposeRenderer(
		const std::string&                       rendererName         , 
		VulkanState&                             vulkanState          , 
		std::shared_ptr<VulkanDescriptorPool>    desctiptorPool       , 
		std::shared_ptr<VulkanDevice>            device               , 
		std::shared_ptr<RendererResourcePool>    rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void SceneComposeRenderer::CreateRendererPass()
	{
		RendererPassBuilder{ "SceneCompose", this }
		.AddSubPass("SceneCompose")
		.AddColorAttachment("SceneColor", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		})
		.AddInputAttachment("Diffuse", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Normal", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Specular", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddInputAttachment("Depth", [&](VkAttachmentDescription& description) {
			description.format = VulkanSwapChain::FindDepthFormat(m_VulkanState.m_PhysicalDevice);
			description.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void SceneComposeRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ "SceneCompose", "SceneCompose", this }
		.AddInput(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "Diffuse", "Normal", "Specular", "Depth" })
		.Build();
	}

	void SceneComposeRenderer::OnSlateResize()
	{
		/**
		* @brief Recreate RenderPass.
		*/
		CreateRendererPass();

		/**
		* @brief Free unused desctiptorSet and descriptorsetlayout.
		*/
		DescriptorSetManager::UnLoad(m_RendererName);

		/**
		* @brief Create descriptorSet again.
		*/
		CreateDescriptorSet();
	}

	void SceneComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass("SceneCompose");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName(m_RendererName));

		builder.BindPipeline("SceneComposeRenderer.Default");
		
		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}