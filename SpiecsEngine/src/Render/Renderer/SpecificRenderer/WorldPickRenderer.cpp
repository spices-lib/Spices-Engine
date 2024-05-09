/**
* @file WorldPickRenderer.cpp.
* @brief The WorldPickRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "WorldPickRenderer.h"

namespace Spiecs {

	namespace WorldPickR {

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
		* @brief This struct contains texture data copyed from Material.
		*/
		struct TextureParams
		{
			/**
			* @brief MeshRenderer allows 1 texture in fragment shader.
			*/
			Renderer::TextureParam params[1];

		public:

			/**
			* @brief Copy data from Material::TextureParam.
			* @param[in] materialTexPars This is variable referenced From Material.
			*/
			void CopyFromMaterial(const std::unordered_map<std::string, Material::TextureParam>& materialTexPars)
			{
				for (auto& pair : materialTexPars)
				{
					params[pair.second.index].CopyFromMaterial(pair.second);
				}
			}
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

	void WorldPickRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
			.CreateCollection<SpecificCollection>()
			.AddPushConstant<WorldPickR::PushConstant>()
			.AddBuffer<WorldPickR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
			.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
			.AddBuffer<WorldPickR::TextureParams>(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
			.Build();
	}

	void WorldPickRenderer::CreatePipeline(VkRenderPass renderPass)
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

	void WorldPickRenderer::Render(FrameInfo& frameInfo)
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

				builder.UpdateBuffer<WorldPickR::TextureParams>(2, 0, [&](auto& ubo) {
					ubo.params[0].constant = glm::vec3(1.0f);
					ubo.params[0].isInUse = 0;
					ubo.params[0].intensity = 1.0f;
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

				builder.UpdateBuffer<WorldPickR::TextureParams>(2, 0, [&](auto& ubo) {
					ubo.CopyFromMaterial(material->GetTextureParams());
				});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet());
			});

			return false;
		});

		builder.EndRenderPass();
	}

	std::unique_ptr<VulkanBuffer>& WorldPickRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_ViewUBO;
		if (set == 2 && binding == 0) return m_TextureParamUBO;

		SPIECS_CORE_ERROR("WorldPickRenderer::Collection:: Out of Range");
		return m_ViewUBO;
	}
}