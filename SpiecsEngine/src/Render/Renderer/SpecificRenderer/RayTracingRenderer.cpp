/**
* @file RayTracingRenderer.cpp.
* @brief The RayTracingRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RayTracingRenderer.h"
#include "PreRenderer.h"

namespace Spiecs {

	RayTracingRenderer::RayTracingRenderer(
		const std::string&                     rendererName          ,
		VulkanState&                           vulkanState           ,
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool        ,
		std::shared_ptr<VulkanDevice>          device                ,
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{}

	void RayTracingRenderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

		RendererPassBuilder{ "RayTracing", this }
		.AddSubPass("RayTracing")
		.EndSubPass()
		.Build();
	}

	void RayTracingRenderer::CreateDescriptorSet()
	{
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracing", this }
		.AddAccelerationStructure(1, 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR)
		.AddStorageTexture<Texture2D>(1, 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR, { "Ray" })
		.Build();
	}

	void RayTracingRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.EndRenderPass();
	}
}