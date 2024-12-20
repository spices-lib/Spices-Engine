/**
* @file SpriteRenderer.cpp.
* @brief The SpriteRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "SpriteRenderer.h"

namespace Spices {

	void SpriteRenderer::CreateRendererPass()
	{
		SPICES_PROFILE_ZONE;

		RendererPassBuilder{ "Sprite", this }
		.AddSubPass("Sprite")
		.AddColorAttachment("SceneColor", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			isEnableBlend                            = true;
			description.format                       = VK_FORMAT_R16G16B16A16_SFLOAT;
		})
		.AddColorAttachment("EntityID", TextureType::Texture2D, [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format                       = VK_FORMAT_R32_SFLOAT;
			description.finalLayout                  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void SpriteRenderer::CreateDescriptorSet()
	{
		SPICES_PROFILE_ZONE;

		DescriptorSetBuilder{ "Sprite", this }
		.AddPushConstant(sizeof(uint64_t))
		.Build();
	}

	void SpriteRenderer::CreatePipeline(
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

	void SpriteRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_ImageIndex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "Sprite" }));
		
		auto [ invViewMatrix, projectionMatrix, stableFrames, fov ] = GetActiveCameraMatrix(frameInfo);

		std::multimap<float, int> sortedEntity;
		IterWorldCompWithBreak<SpriteComponent>(frameInfo, [&](int entityId, TransformComponent& tranComp, SpriteComponent& spriteComp) {

			auto viewPos = glm::inverse(invViewMatrix) * glm::vec4(tranComp.GetPosition(), 0.0f);

			sortedEntity.insert(std::pair(viewPos.z, entityId));

			return false;
		});

		for (auto it = sortedEntity.rbegin(); it != sortedEntity.rend(); ++it)
		{
			auto [transComp, spriteComp] = frameInfo.m_World->GetRegistry().get<TransformComponent, SpriteComponent>(static_cast<entt::entity>(it->second));

			spriteComp.GetMesh()->Draw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshPackId, auto meshPack) {
				
				builder.BindPipeline(meshPack->GetMaterial()->GetName());

				builder.UpdatePushConstant<uint64_t>([&](auto& push) {
					push = meshPack->GetMeshDesc().GetBufferAddress();
				});
			});
		}

		builder.EndRenderPass();
	}
}