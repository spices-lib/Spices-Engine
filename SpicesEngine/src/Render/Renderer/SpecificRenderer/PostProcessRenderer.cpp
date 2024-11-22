/**
* @file PostProcessRenderer.cpp.
* @brief The PostProcessRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PostProcessRenderer.h"

namespace Spices {

	namespace PostProcessR
	{
		struct BloomPushConstant
		{
			uint32_t mipmap;
			float weight;
		};
	}

	void PostProcessRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "PostProcess", this }
		.AddSubPass("DownSample")
		.EndSubPass()
		.AddSubPass("Bloom")
		.EndSubPass()
		/*.AddSubPass("Tonemapping")
		.EndSubPass()*/
		.Build();
	}

	void PostProcessRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		auto image = m_RendererResourcePool->AccessRowResource("SceneColor").get();
		
		DescriptorSetBuilder{ "DownSample", this }
		.AddPushConstant(sizeof(uint32_t))
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "SceneColor" })
		.AddStorageTextureMipmaps(2, 1, VK_SHADER_STAGE_COMPUTE_BIT, "Bloom", image->GetFormat(), TextureType::Texture2D, [&](RendererResourceCreateInfo& info) {
			info.mipLevel  = SpicesShader::POST_BLOOM_MIPMAP;
			info.sizeScale = 0.5f;
		})
		.Build();

		DescriptorSetBuilder{ "Bloom", this }
		.AddPushConstant(sizeof(PostProcessR::BloomPushConstant))
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "SceneColor" })
		.AddStorageTextureMipmaps(2, 1, VK_SHADER_STAGE_COMPUTE_BIT, "Bloom", image->GetFormat(), TextureType::Texture2D, [&](RendererResourceCreateInfo& info) {
			info.mipLevel  = SpicesShader::POST_BLOOM_MIPMAP;
			info.sizeScale = 0.5f;
		})
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

		auto image = m_RendererResourcePool->AccessRowResource("Bloom").get();

		ComputeRenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex, m_VulkanState.m_GraphicCommandBuffer };

		builder.BeginRenderPass();
		
		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "DownSample" }));

		builder.BindPipeline("PostProcessRenderer.DownSample.Default");

		for (int i = 0; i < image->GetMipLevels(); i++)
		{
			builder.UpdatePushConstant<uint32_t>([&](auto& push) {
				push = i;
			});

			builder.Dispatch((image->GetWidth() >> i) / 32 + 1, (image->GetHeight() >> i) / 32 + 1, 1);

			//builder.InternalBarriers();
		}

		builder.EndRecording();

		builder.BeginNextSubPass("Bloom");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Bloom" }));

		builder.BindPipeline("PostProcessRenderer.Bloom.Default");

		for (int i = image->GetMipLevels() - 1; i >= 0; i--)
		{
			builder.UpdatePushConstant<PostProcessR::BloomPushConstant>([&](auto& push) {
				push.mipmap = i;
				push.weight = i == 0 ? 0.08f : 1.0f;
			});

			builder.Dispatch(((image->GetWidth() * 2) >> i) / 32 + 1, ((image->GetHeight() * 2) >> i) / 32 + 1, 1);

			if (i != 0)
			{
				//builder.InternalBarriers();
			}
		}

		builder.EndRenderPass();
	}
}