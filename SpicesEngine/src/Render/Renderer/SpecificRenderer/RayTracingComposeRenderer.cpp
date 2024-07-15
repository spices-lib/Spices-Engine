/**
* @file RayTracingComposeRenderer.cpp.
* @brief The RayTracingComposeRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RayTracingComposeRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spices {

	RayTracingComposeRenderer::RayTracingComposeRenderer(
		const std::string&                           rendererName        ,
		VulkanState&                                 vulkanState         ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool      ,
		const std::shared_ptr<VulkanDevice>&         device              ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool)
	{
		SPICES_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void RayTracingComposeRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "RayTracingCompose", this }
		.AddSubPass("RayTracingCompose")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
			description.finalLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.Build();
	}

	void RayTracingComposeRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracingCompose", this }
		.AddStorageTexture(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "Ray" })
		.Build();
	}

	void RayTracingComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "RayTracingCompose" }));

		builder.BindPipeline("RayTracingComposeRenderer.RayTracingCompose.Default");

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}