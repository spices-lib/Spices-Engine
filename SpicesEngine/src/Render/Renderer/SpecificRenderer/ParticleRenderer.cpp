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

		if (!FrameInfo::Get().m_World)
		{
			DescriptorSetBuilder{ "Particle", this }
			.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "Particle" }, VK_FORMAT_R32G32B32A32_SFLOAT)
			.AddStorageBuffer(2, 1, sizeof(SpicesShader::Vertex), VK_SHADER_STAGE_COMPUTE_BIT)
			.Build();

			return;
		}

		std::shared_ptr<VulkanBuffer> buffer = nullptr;

		IterWorldCompWithBreak<TagComponent>(FrameInfo::Get(), [&](int entityID, TransformComponent& tranComp, TagComponent& tagComp) {
			if (tagComp.GetTag().find("Ground") != tagComp.GetTag().end())
			{
				auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<MeshComponent>((entt::entity)entityID);
				std::shared_ptr<MeshPack> meshPack = *meshComp.GetMesh()->GetPacks().first();
				buffer = meshPack->GetResource().positions.buffer;
				return true;
			}
			return false;
		});
		
		if (!buffer)
		{
			DescriptorSetBuilder{ "Particle", this }
			.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "Particle" }, VK_FORMAT_R32G32B32A32_SFLOAT)
			.AddStorageBuffer(2, 1, sizeof(SpicesShader::Vertex), VK_SHADER_STAGE_COMPUTE_BIT)
			.Build();

			return;
		}

		DescriptorSetBuilder{ "Particle", this }
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "Particle" }, VK_FORMAT_R32G32B32A32_SFLOAT)
		.AddStorageBuffer(2, 1, VK_SHADER_STAGE_COMPUTE_BIT, buffer)
		.Build();
	}

	void ParticleRenderer::OnMeshAddedWorld()
	{
		SPICES_PROFILE_ZONE;

		/**
		* @breif ReCreate RenderPass, DescriptorSet and DefaultMaterial.
		*/
		Renderer::OnSystemInitialize();
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

		builder.Recording("Particle");

		std::shared_ptr<VulkanBuffer> buffer = nullptr;

		IterWorldCompWithBreak<TagComponent>(FrameInfo::Get(), [&](int entityID, TransformComponent& tranComp, TagComponent& tagComp) {
			if (tagComp.GetTag().find("Ground") != tagComp.GetTag().end())
			{
				auto& meshComp = FrameInfo::Get().m_World->GetRegistry().get<MeshComponent>((entt::entity)entityID);
				std::shared_ptr<MeshPack> meshPack = *meshComp.GetMesh()->GetPacks().first();
				buffer = meshPack->GetResource().positions.buffer;
				return true;
			}
			return false;
		});

		if (!buffer) return;

		//builder.AddBarriers(buffer->Get(), 0, VK_ACCESS_SHADER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Particle" }));

		builder.BindPipeline("ParticleRenderer.Particle.Default");

		builder.Dispatch(16 * 16, 1, 1);
		
		//builder.ReleaseBarriers(buffer->Get(), VK_ACCESS_SHADER_WRITE_BIT, 0, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

		builder.Endrecording();
	}
}