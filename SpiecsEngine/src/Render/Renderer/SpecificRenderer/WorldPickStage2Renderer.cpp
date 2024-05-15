/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "WorldPickStage2Renderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	WorldPickStage2Renderer::WorldPickStage2Renderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool, std::shared_ptr<VulkanDevice> device, std::shared_ptr<RendererResourcePool> rendererResourcePool)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void WorldPickStage2Renderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		m_RenderPass->AddColorAttachment("SceneColor", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		m_RenderPass->Build();

		/**
		* @brief create renderresource's descriptorset.
		*/
		m_RendererResourcePool->AccessRowResource("SelectBuffer")->CreateDescriptorSet(0);
	}

	void WorldPickStage2Renderer::CreateDescriptorSet()
	{
		PipelineLayoutBuilder{ this }
		.AddPushConstant<WorldPickStage2R::PushConstant>()
		.AddTexture<Texture2D>(0, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void WorldPickStage2Renderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		ImVec2 gbufferSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();
		VkExtent2D windowSize = m_Device->GetSwapChainSupport().surfaceSize;
		builder.UpdatePushConstant<WorldPickStage2R::PushConstant>([&](auto& push) {
			push.gbufferSize = { gbufferSize.x, gbufferSize.y, 1.0f / gbufferSize.x, 1.0f / gbufferSize.y };
			push.windowSize = { windowSize.width, windowSize.height, 1.0f / windowSize.width, 1.0 / windowSize.height };
		});

		builder.BindDescriptorSet(0, m_RendererResourcePool->AccessRowResource("SelectBuffer")->GetDescriptorSet());

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}