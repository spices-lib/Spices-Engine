/**
* @file PostProcessRenderer.cpp.
* @brief The PostProcessRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PostProcessRenderer.h"

namespace Spices {

	void PostProcessRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "PostProcess", this }
		.AddSubPass("DownSample")
		.EndSubPass()
		.AddSubPass("Blur")
		.EndSubPass()
		.AddSubPass("TAA")
		.EndSubPass()
		.AddSubPass("Tonemapping")
		.EndSubPass()
		.Build();
	}

	void PostProcessRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Particle", this }
		.Build();
	}

	std::shared_ptr<VulkanPipeline> PostProcessRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};

		pipelineConfig.pipelineLayout = layout;

		return std::make_shared<VulkanComputePipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	void PostProcessRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

	}
}