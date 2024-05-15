/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "WorldPickRenderer.h"

namespace Spiecs {

	void WorldPickRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add SelectBuffer Attachment.
		* Though we want use SelectBuffer with a sampler, we need transfrom shaderread layout here.
		*/
		m_RenderPass->AddColorAttachment("SelectBuffer", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format = VK_FORMAT_R32_SFLOAT;
		});

		m_RenderPass->Build();
	}

	void WorldPickRenderer::CreateDescriptorSet()
	{
		PipelineLayoutBuilder{ this }
		.CreateCollection<SpecificCollection>()
		.AddPushConstant<WorldPickR::PushConstant>()
		.AddBuffer<WorldPickR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
		.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void WorldPickRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.UpdateBuffer<WorldPickR::View>(0, 0, [&](auto& ubo) {
			auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ubo.view = glm::inverse(invViewMatrix);
			ubo.projection = projectionMatrix;
		});

		IterWorldComp<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			if (frameInfo.m_PickEntityID.find(entityId) == frameInfo.m_PickEntityID.end()) return false;

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.UpdatePushConstant<WorldPickR::PushConstant>([&](auto& push) {
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
				builder.UpdatePushConstant<WorldPickR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet()[0]);
			});

			return false;
		});

		builder.EndRenderPass();
	}
}