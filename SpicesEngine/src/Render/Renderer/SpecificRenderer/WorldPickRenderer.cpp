/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "WorldPickRenderer.h"

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
		.AddPushConstant<SpicesShader::PushConstantMesh>()
		.Build();
	}

	void WorldPickRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPICES_PROFILE_ZONE;

		RenderBehaveBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "WorldPick" }));

		IterWorldCompWithBreak<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			if (!frameInfo.m_PickEntityID.has_key(entityId)) return false;

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline("WorldPickRenderer.WorldPick.Default");

				builder.UpdatePushConstant<SpicesShader::PushConstantMesh>([&](auto& push) {
					push.model = modelMatrix;
					push.desc.materialParameterAddress = material->GetMaterialParamsAddress();
					push.desc.entityID = entityId;
				});
			});

			return false;
		});

		IterWorldCompWithBreak<SpriteComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SpriteComponent& spriteComp) {
			if (!frameInfo.m_PickEntityID.has_key(entityId)) return false;

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			spriteComp.GetMesh()->Draw(m_VulkanState.m_GraphicCommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline("WorldPickRenderer.WorldPick.Default");

				builder.UpdatePushConstant<SpicesShader::PushConstantMesh>([&](auto& push) {
					push.model = modelMatrix;
					push.desc.materialParameterAddress = material->GetMaterialParamsAddress();
					push.desc.entityID = entityId;
				});
			});

			return false;
		});

		builder.EndRenderPass();
	}
}