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
		/*.AddSubPass("DownSample")
		.EndSubPass()*/
		.AddSubPass("Blur")
		.EndSubPass()
		/*.AddSubPass("TAA")
		.EndSubPass()
		.AddSubPass("Tonemapping")
		.EndSubPass()*/
		.Build();
	}

	void PostProcessRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Blur", this }
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "SceneColor" })
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

		ComputeRenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };

		//builder.Recording("Blur");

		auto image = m_RendererResourcePool->AccessRowResource("SceneColor").get();

		builder.AddBarriers(image, VK_ACCESS_NONE, VK_ACCESS_SHADER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Blur" }));

		builder.BindPipeline("PostProcessRenderer.Blur.Default");
		
		builder.Dispatch(32, 32, 1);

		builder.ReleaseBarriers(image, VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_NONE, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

		//builder.EndRecording();
	}
}