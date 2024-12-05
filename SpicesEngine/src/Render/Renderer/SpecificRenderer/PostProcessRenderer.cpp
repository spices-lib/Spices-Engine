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
			uint64_t materialAddress;
			uint32_t mipmap;
			float    weight;
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
		.AddSubPass("TAA")
		.EndSubPass()
		.AddSubPass("ToneMapping")
		.EndSubPass()
		.Build();
	}

	void PostProcessRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		const auto image = m_RendererResourcePool->AccessRowResource("SceneColor").get();
		
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

		DescriptorSetBuilder{ "TAA", this }
		.Build();

		DescriptorSetBuilder{ "ToneMapping", this }
		.AddPushConstant(sizeof(uint64_t))
		.AddStorageTexture(2, 0, VK_SHADER_STAGE_COMPUTE_BIT, { "SceneColor" })
		.Build();
	}

	void PostProcessRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, material, this }
		.SetPipelineLayout(layout)
		.BuildCompute();
	}

	void PostProcessRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		const auto bloomImage = m_RendererResourcePool->AccessRowResource("Bloom").get();

		ComputeRenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex, m_VulkanState.m_GraphicCommandBuffer };

		builder.BeginRenderPass();
		
		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "DownSample" }));

		builder.BindPipeline("PostProcessRenderer.DownSample.Default");

		for (uint32_t i = 0; i < bloomImage->GetMipLevels(); i++)
		{
			builder.UpdatePushConstant<uint32_t>([&](auto& push) {
				push = i;
			});

			builder.Dispatch((bloomImage->GetWidth() >> i) / 32 + 1, (bloomImage->GetHeight() >> i) / 32 + 1, 1);

			builder.InternalBarriers();
		}

		builder.BeginNextSubPass("Bloom");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Bloom" }));

		builder.BindPipeline("PostProcessRenderer.Bloom.Default");

		for (int i = bloomImage->GetMipLevels() - 1; i >= 0; i--)
		{
			builder.UpdatePushConstant<PostProcessR::BloomPushConstant>([&](auto& push) {
				push.materialAddress = GetDefaultMaterial("Bloom")->GetMaterialParamsAddress();
				push.mipmap = i;
				push.weight = i == 0 ? 0.08f : 1.0f;
			});

			builder.Dispatch(((bloomImage->GetWidth() * 2) >> i) / 32 + 1, ((bloomImage->GetHeight() * 2) >> i) / 32 + 1, 1);

			builder.InternalBarriers();
		}

		builder.BeginNextSubPass("TAA");

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "TAA" }));

		builder.BindPipeline("PostProcessRenderer.TAA.Default");

		builder.Dispatch(1, 1, 1);

		builder.InternalBarriers();

		builder.BeginNextSubPass("ToneMapping");

		const auto sceneColor = m_RendererResourcePool->AccessRowResource("SceneColor").get();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "ToneMapping" }));

		builder.BindPipeline("PostProcessRenderer.ToneMapping.Default");

		builder.UpdatePushConstant<uint64_t>([&](auto& push) {
			push = GetDefaultMaterial("ToneMapping")->GetMaterialParamsAddress();
		});

		builder.Dispatch(sceneColor->GetWidth() / 32 + 1, sceneColor->GetHeight() / 32 + 1, 1);

		builder.InternalBarriers();

		builder.EndRenderPass();
	}
}