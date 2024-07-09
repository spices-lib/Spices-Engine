/**
* @file RayTracingComposeRenderer.cpp.
* @brief The RayTracingComposeRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RayTracingComposeRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	RayTracingComposeRenderer::RayTracingComposeRenderer(
		const std::string&                       rendererName        ,
		VulkanState&                             vulkanState         ,
		std::shared_ptr<VulkanDescriptorPool>    descriptorPool      ,
		std::shared_ptr<VulkanDevice>            device              ,
		std::shared_ptr<RendererResourcePool>    rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool)
	{
		SPIECS_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void RayTracingComposeRenderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracingCompose", this }
		.AddStorageTexture(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "Ray" })
		.Build();
	}

	void RayTracingComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		RenderBehaverBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

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