/**
* @file ShadowRenderer.cpp.
* @brief The ShadowRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "ShadowRenderer.h"
#include "PreRenderer.h"
#include "Systems/SlateSystem.h"

namespace Spiecs {

	ShadowRenderer::ShadowRenderer(
		const std::string&                     rendererName         ,
		VulkanState&                           vulkanState          ,
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool       ,
		std::shared_ptr<VulkanDevice>          device               ,
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		SPIECS_PROFILE_ZONE;

		m_Square = std::make_unique<SquarePack>();
		m_Square->OnCreatePack();
	}

	void ShadowRenderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

		RendererPassBuilder{ "Shadow", this }
		.AddSubPass("DirectionalLightShadow")
		.AddDepthAttachment("DirectionalLightShadow", TextureType::Texture2DArray, [](VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		})
		.EndSubPass()
		.Build();
	}

	void ShadowRenderer::CreateDescriptorSet()
	{
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "DirectionalLightShadow", this }
		.Build();
	}

	void ShadowRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindPipeline("ShadowRenderer.DirectionalLightShadow.Default");

		m_Square->OnBind(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		m_Square->OnDraw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

		builder.EndRenderPass();
	}
}