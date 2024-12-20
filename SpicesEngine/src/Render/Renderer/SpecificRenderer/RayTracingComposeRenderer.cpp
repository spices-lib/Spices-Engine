/**
* @file RayTracingComposeRenderer.cpp.
* @brief The RayTracingComposeRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "RayTracingComposeRenderer.h"

namespace Spices {

	RayTracingComposeRenderer::RayTracingComposeRenderer(
		const std::string&                           rendererName        ,
		VulkanState&                                 vulkanState         ,
		const std::shared_ptr<VulkanDescriptorPool>& descriptorPool      ,
		const std::shared_ptr<VulkanDevice>&         device              ,
		const std::shared_ptr<RendererResourcePool>& rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool)
	{}

	void RayTracingComposeRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "RayTracingCompose", this }
		.AddSubPass("RayTracingCompose")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format            = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("EntityID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format            = VK_FORMAT_R32_SFLOAT;
		})
		.AddColorAttachment("TriangleID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.finalLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.AddDepthAttachment("Depth", TextureType::Texture2D, [](VkAttachmentDescription& description) {
			description.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;   /* @attention It seams that layout transform is not work? */
			description.loadOp            = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.Build();
	}

	void RayTracingComposeRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracingCompose", this }
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT, { "RayImage" })
		.AddStorageTexture(2, 1, VK_SHADER_STAGE_FRAGMENT_BIT, { "RayEntityID", "RayTriangleID" })
		.Build();
	}

	void RayTracingComposeRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, material, this }
		.SetDefault()
		.NullBindingDescriptions()
		.NullAttributeDescriptions()
		.SetRenderPass()
		.SetSubPassIndex()
		.SetPipelineLayout(layout)
		.SetCullMode(VK_CULL_MODE_NONE)
		.SetColorAttachments()
		.Build();
	}

	void RayTracingComposeRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		if(frameInfo.m_RendererType != RendererType::PathTracing) return;
		
		RenderBehaveBuilder builder{ this, frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };

		builder.BeginRenderPass();

		builder.SetViewPort();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "RayTracingCompose" }));

		builder.BindPipeline("RayTracingComposeRenderer.RayTracingCompose.Default");

		builder.DrawFullScreenTriangle();

		builder.EndRenderPass();
	}
}