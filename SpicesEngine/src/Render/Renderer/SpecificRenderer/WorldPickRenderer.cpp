/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "WorldPickRenderer.h"
#include "World/Components/EntityComponent.h"

namespace Spices {

	void WorldPickRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "WorldPick", this }
		.AddSubPass("WorldPick")
		.AddColorAttachment("SelectBuffer", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout                         = VK_IMAGE_LAYOUT_UNDEFINED;
			description.finalLayout                           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.loadOp                                = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format                                = VK_FORMAT_R32_SFLOAT;
		})
		.EndSubPass()
		.Build();
	}

	void WorldPickRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "WorldPick", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();
	}

	void WorldPickRenderer::CreatePipeline(
		std::shared_ptr<Material>        material ,
		VkPipelineLayout&                layout   ,
		std::shared_ptr<RendererSubPass> subPass
	)
	{
		SPICES_PROFILE_ZONE;

		PipelineBuilder{ subPass, material, this }
		.SetDefault()
		.SetRenderPass()
		.SetSubPassIndex()
		.SetPipelineLayout(layout)
		.SetCullMode(VK_CULL_MODE_NONE)
		.SetColorAttachments()
		.Build();
	}

	void WorldPickRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "WorldPick" }));

		static std::function<void(Entity)> drawSelected = [&](Entity e) {

			if (e.HasComponent<MeshComponent>())
			{
				MeshComponent& meshComp = e.GetComponent<MeshComponent>();

				meshComp.GetMesh()->Draw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto meshPack) {
					builder.BindPipeline("WorldPickRenderer.WorldPick.Default");

					builder.UpdatePushConstant<uint64_t>([&](auto& push) {
						push = meshPack->GetMeshDesc().GetBufferAddress();
					});
				});
			}

			if (e.HasComponent<SpriteComponent>())
			{
				SpriteComponent& meshComp = e.GetComponent<SpriteComponent>();

				meshComp.GetMesh()->Draw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto meshPack) {
					builder.BindPipeline("WorldPickRenderer.WorldPick.Default");

					builder.UpdatePushConstant<uint64_t>([&](auto& push) {
						push = meshPack->GetMeshDesc().GetBufferAddress();
					});
				});
			}
		};

		frameInfo.m_PickEntityID.for_each([&](const auto& k, const auto& v) {
			
			Entity e = frameInfo.m_World->QueryEntitybyID(k);

			drawSelected(e);

			if (e.HasComponent<EntityComponent>())
			{
				EntityComponent& entityComp = e.GetComponent<EntityComponent>();

				for (auto& entity : entityComp.GetEntities())
				{
					Entity child = frameInfo.m_World->QueryEntitybyID(entity);
					drawSelected(child);
				}
			}

			return false;
		});

		builder.EndRenderPass();
	}
}