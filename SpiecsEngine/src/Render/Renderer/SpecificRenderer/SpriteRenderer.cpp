/**
* @file SpriteRenderer.cpp.
* @brief The SpriteRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SpriteRenderer.h"
#include "PreRenderer.h"

namespace Spiecs {

	void SpriteRenderer::CreateRendererPass()
	{
		RendererPassBuilder{ "Sprite", this }
		.AddSubPass("Sprite")
		.AddColorAttachment("SceneColor", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			isEnableBlend = true;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; //temp
		})
		.AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32_SFLOAT;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		})
		.EndSubPass()
		.Build();
	}

	void SpriteRenderer::CreateDescriptorSet()
	{
		DescriptorSetBuilder{ "Sprite", this }
		.AddPushConstant<PreR::PushConstant>()
		.Build();
	}

	void SpriteRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("PreRenderer"));

		builder.BindDescriptorSet(DescriptorSetManager::GetByName(m_RendererName));

		glm::vec3 camPos;
		auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
		camPos = glm::vec3(invViewMatrix[3][0], invViewMatrix[3][1], invViewMatrix[3][2]);

		std::map<float, int> sortedEntity;
		IterWorldComp<SpriteComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SpriteComponent& spriteComp) {
			glm::vec3 dis = transComp.GetPosition() - camPos;
			sortedEntity[glm::dot(dis, dis)] = entityId;

			return false;
		});

		for (auto it = sortedEntity.rbegin(); it != sortedEntity.rend(); ++it)
		{
			auto& [transComp, spriteComp] = frameInfo.m_World->GetRegistry().get<TransformComponent, SpriteComponent>((entt::entity)it->second);

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			spriteComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.BindPipeline(material->GetName());

				builder.UpdatePushConstant<PreR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = it->second;
				});

				builder.BindDescriptorSet(material->GetMaterialDescriptorSet());
			});
		}

		builder.EndRenderPass();
	}
}