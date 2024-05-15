/**
* @file SpriteRenderer.cpp.
* @brief The SpriteRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SpriteRenderer.h"

namespace Spiecs {

	void SpriteRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add SceneColor Attachment.
		* Though we want use SelectBuffer with a sampler, we need transfrom shaderread layout here.
		*/
		m_RenderPass->AddColorAttachment("SceneColor", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			isEnableBlend = true;
		});

		/**
		* @brief Add ID Attachment.
		*/
		m_RenderPass->AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32_SFLOAT;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});
		
		m_RenderPass->Build();
	}

	void SpriteRenderer::CreateDescriptorSet()
	{
		PipelineLayoutBuilder{ this }
			.CreateCollection<SpecificCollection>()
			.AddPushConstant<SpriteR::PushConstant>()
			.AddBuffer<SpriteR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
			.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
			.Build();
	}

	void SpriteRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		glm::vec3 camPos;
		builder.UpdateBuffer<SpriteR::View>(0, 0, [&](auto& ubo) {
			auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			camPos = glm::vec3(invViewMatrix[3][0], invViewMatrix[3][1] , invViewMatrix[3][2] );
			ubo.view = glm::inverse(invViewMatrix);
			ubo.projection = projectionMatrix;
		});

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
				builder.UpdatePushConstant<SpriteR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = it->second;
				});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet()[0]);
			});
		}

		builder.EndRenderPass();
	}
}