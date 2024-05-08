/**
* @file SpriteRenderer.cpp.
* @brief The SpriteRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SpriteRenderer.h"

namespace Spiecs {

	namespace SpriteR {

		/**
		* @brief This struct is specific MeshRenderer PsuhConstant
		*/
		struct PushConstant
		{
			/**
			* @brief Meshpack ModelMatrix.
			*/
			glm::mat4 model = glm::mat4(1.0f);

			/**
			* @brief Entityid, cast from entt::entity.
			*/
			int entityID = -1;
		};

		/**
		* @brief VertexShader Stage uniform buffer data.
		*/
		struct View
		{
			/**
			* @brief Projection Matrix.
			*/
			glm::mat4 projection = glm::mat4(1.0f);

			/**
			* @brief View Matrix.
			*/
			glm::mat4 view = glm::mat4(1.0f);
		};

	}

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
		m_RenderPass->AddColorAttachment("SceneColor", [](VkAttachmentDescription& description) {
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		});

		m_RenderPass->Build();
	}

	void SpriteRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
			.CreateCollection<SpecificCollection>()
			.AddPushConstant<SpriteR::PushConstant>()
			.AddBuffer<SpriteR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
			.Build();
	}

	void SpriteRenderer::CreatePipeline(VkRenderPass renderPass)
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		pipelineConfig.colorBlendInfo.attachmentCount = (uint32_t)m_RenderPass->GetColorBlend().size();
		pipelineConfig.colorBlendInfo.pAttachments = m_RenderPass->GetColorBlend().data();
		m_VulkanPipeline = std::make_unique<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath("vert"),
			GetSahderPath("frag"),
			pipelineConfig
		);
	}

	void SpriteRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.UpdateBuffer<SpriteR::View>(0, 0, [&](auto& ubo) {
			auto& [viewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ubo.view = viewMatrix;
			ubo.projection = projectionMatrix;
		});

		/*IterWorldComp<Component>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			if (frameInfo.m_PickEntityID.find(entityId) == frameInfo.m_PickEntityID.end()) return false;

			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.UpdatePushConstant<WorldPickR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
					});
				});

			return false;
		});*/

		builder.EndRenderPass();
	}

	std::unique_ptr<VulkanBuffer>& SpriteRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_ViewUBO;

		SPIECS_CORE_ERROR("SpriteRenderer::Collection:: Out of Range");
		return m_ViewUBO;
	}
}