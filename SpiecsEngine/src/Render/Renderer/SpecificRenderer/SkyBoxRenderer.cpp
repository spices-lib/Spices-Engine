/**
* @file SkyBoxRenderer.cpp.
* @brief The SkyBoxRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "SkyBoxRenderer.h"

namespace Spiecs {

	namespace SkyBoxR {

		/**
		* @brief This struct is specific SkyBoxRenderer PsuhConstant
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

	void SkyBoxRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add Albedo Attachment.
		*/
		m_RenderPass->AddColorAttachment("Diffuse", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Add ID Attachment.
		*/
		m_RenderPass->AddColorAttachment("ID", [](bool& isEnableBlend, VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			description.format = VK_FORMAT_R32_SFLOAT;
		});

		/**
		* @brief Add Depth Attachment.
		*/
		m_RenderPass->AddDepthAttachment([](VkAttachmentDescription& description) {
			description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		});

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void SkyBoxRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.CreateCollection<SpecificCollection>()
		.AddPushConstant<SkyBoxR::PushConstant>()
		.AddBuffer<SkyBoxR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
		.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void SkyBoxRenderer::CreatePipeline(VkRenderPass renderPass)
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_FRONT_BIT;
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

	void SkyBoxRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.UpdateBuffer<SkyBoxR::View>(0, 0, [&](auto& ubo) {
			auto& [invViewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ubo.view = glm::inverse(invViewMatrix);
			ubo.projection = projectionMatrix;
			});

		IterWorldComp<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			skyboxComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.UpdatePushConstant<SkyBoxR::PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
				});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet());
			});
				
			return false;
		});

		builder.EndRenderPass();
	}

	std::unique_ptr<VulkanBuffer>& SkyBoxRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_ViewUBO;

		SPIECS_CORE_ERROR("SkyBoxRenderer::Collection:: Out of Range");
		return m_ViewUBO;
	}
}