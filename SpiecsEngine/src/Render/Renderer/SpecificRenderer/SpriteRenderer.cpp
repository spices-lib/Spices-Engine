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
			description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
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

	void SpriteRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
			.CreateCollection<SpecificCollection>()
			.AddPushConstant<SpriteR::PushConstant>()
			.AddBuffer<SpriteR::View>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
			.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
			.AddBuffer<SpriteR::TextureParams>(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
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

				builder.UpdateBuffer<SpriteR::TextureParams>(2, 0, [&](auto& ubo) {
					ubo.CopyFromMaterial(material->GetTextureParams());
				});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet());
			});
		}

		builder.EndRenderPass();
	}

	std::unique_ptr<VulkanBuffer>& SpriteRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_ViewUBO;
		if (set == 2 && binding == 0) return m_TextureParamUBO;

		SPIECS_CORE_ERROR("SpriteRenderer::Collection:: Out of Range");
		return m_ViewUBO;
	}
}