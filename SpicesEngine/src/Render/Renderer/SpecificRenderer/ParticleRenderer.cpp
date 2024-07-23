/**
* @file ParticleRenderer.cpp.
* @brief The ParticleRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "ParticleRenderer.h"

namespace Spices {

	void ParticleRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "Particle", this }
		.AddSubPass("Particle")
		.EndSubPass()
		.Build();
	}

	void ParticleRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Particle", this }
		.AddStorageTexture(1, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "Particle" }, VK_FORMAT_R32G32B32A32_SFLOAT)
		.Build();
	}

	std::shared_ptr<VulkanPipeline> ParticleRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};

		pipelineConfig.pipelineLayout              = layout;

		return std::make_shared<VulkanComputePipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	void ParticleRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		ComputeRenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.Recording("RayTracing");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Particle" }));

		builder.BindPipeline("ParticleRenderer.Particle.Default");

		builder.Dispatch(16, 1, 1);
		
		builder.Endrecording();
	}
}