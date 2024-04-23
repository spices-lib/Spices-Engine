/**
* @file MeshRenderer.cpp.
* @brief The MeshRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"

namespace Spiecs {

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

		/**
		* @brief Meshpackid, from arrayindex of meshpack.
		*/
		int meshpackID = -1;
	};

	/**
	* @brief This struct contains texture data copyed from Material.
	*/
	struct TextureParams
	{
		/**
		* @brief MeshRenderer allows 3 texture in fragment shader.
		*/
		Renderer::TextureParam params[3];

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
	struct VertRendererUBO
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

	/**
	* @brief PointLight Render Data.
	*/
	struct PointLightUBO
	{
		/**
		* @brief Each data is copyed from PointLightComponent.
		*/
		std::array<PointLightComponent::PointLight, 10> pointLights;
	};

	void MeshRenderer::CreateRenderPass()
	{
		/**
		* @brief Declear an empty VulkanRenderPass Object.
		*/
		m_RenderPass = std::make_unique<VulkanRenderPass>(m_VulkanState, m_Device, m_RendererResourcePool);

		/**
		* @brief Add SwapChian Attachment.
		*/
		m_RenderPass->AddSwapChainAttachment([](VkAttachmentDescription& description) {
		});

		/**
		* @brief Add Normal Attachment.
		*/
		m_RenderPass->AddColorAttachment("Normal", [](VkAttachmentDescription& description) {
		});

		/**
		* @brief Add ID Attachment.
		*/
		m_RenderPass->AddColorAttachment("ID", [](VkAttachmentDescription& description) {
			description.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		});

		/**
		* @brief Add Depth Attachment.
		*/
		m_RenderPass->AddDepthAttachment([](VkAttachmentDescription& description) {
		});

		/**
		* @brief Create VkRenderPass, Resource, FrameBuffer.
		*/
		m_RenderPass->Build();
	}

	void MeshRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.CreateCollection<SpecificCollection>()
		.AddPushConstant<PushConstant>()
		.AddBuffer<VertRendererUBO>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
		.AddTexture<Texture2D>(1, 0, 3, VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<TextureParams>(2, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<DirectionalLightComponent::DirectionalLight>(2, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<PointLightUBO>(2, 2, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void MeshRenderer::CreatePipeline(VkRenderPass renderPass)
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

	void MeshRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex };

		builder.UpdateBuffer<VertRendererUBO>(0, 0, [&](auto& ubo) {
			auto& [viewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ubo.view = viewMatrix;
			ubo.projection = projectionMatrix;
		});

		builder.UpdateBuffer<DirectionalLightComponent::DirectionalLight>(2, 1, [&](auto& ubo) {
			ubo = GetDirectionalLight(frameInfo);
		});

		builder.UpdateBuffer<PointLightUBO>(2, 2, [&](auto& ubo) {
			ubo.pointLights = GetPointLight(frameInfo);
		});

		IterWorldComp<MeshComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, MeshComponent& meshComp) {
			const glm::mat4& modelMatrix = transComp.GetModelMatrix();

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.UpdatePushConstant<PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
					push.meshpackID = meshpackId;
				});

				builder.UpdateBuffer<TextureParams>(2, 0, [&](auto& ubo) {
					ubo.CopyFromMaterial(material->GetTextureParams());
				});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet());
			});

			return false;
		});

		builder.EndRenderPass();
	}

	std::unique_ptr<VulkanBuffer>& MeshRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_VertRendererUBO;
		if (set == 2 && binding == 0) return m_TextureParamUBO;
		if (set == 2 && binding == 1) return m_DirectionalLightUBO;
		if (set == 2 && binding == 2) return m_PointLightUBO;
		
		return m_VertRendererUBO;
		__debugbreak();
		SPIECS_LOG("Out of Range");
	}
}