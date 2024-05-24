/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "WorldPickRenderer.h"
#include "PreRenderer.h"

namespace Spiecs {

	void WorldPickRenderer::CreateRendererPass()
	{
		RendererPassBuilder{ "WorldPick", this }
		.AddSubPass("WorldPick")
		.AddColorAttachment("SelectBuffer", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.finalLayout   = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format        = VK_FORMAT_R32_SFLOAT;
		})
		.EndSubPass()
		.Build();
	}

	void WorldPickRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ "WorldPick", this }
		.AddPushConstant<PreR::PushConstant>()
		.Build();
	}

	void WorldPickRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName({ m_Pass->GetName(), "WorldPick" }));

		IterWorldComp<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			if (frameInfo.m_PickEntityID.find(entityId) == frameInfo.m_PickEntityID.end()) return false;

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline("WorldPickRenderer.WorldPick.Default");

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});
			});

			return false;
		});

		IterWorldComp<SpriteComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SpriteComponent& spriteComp) {
			if (frameInfo.m_PickEntityID.find(entityId) == frameInfo.m_PickEntityID.end()) return false;

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			spriteComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline("WorldPickRenderer.WorldPick.Default");

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});
			});

			return false;
		});

		builder.EndRenderPass();
	}
}