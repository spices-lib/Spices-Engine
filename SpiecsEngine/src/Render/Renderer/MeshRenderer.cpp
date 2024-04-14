#include "Pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"

namespace Spiecs {

	struct PushConstant
	{
		glm::mat4 model = glm::mat4(1.0f);
		int entityID = -1;
		int meshpackID = -1;
	};

	struct TextureParams
	{
		Renderer::TextureParam params[3];

	public:
		void CopyFromMaterial(const std::unordered_map<std::string, Material::TextureParam>& materialTexPars)
		{
			for (auto& pair : materialTexPars)
			{
				params[pair.second.index].CopyFromMaterial(pair.second);
			}
		}
	};

	struct VertRendererUBO
	{
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	struct PointLightUBO
	{
		std::array<PointLightComponent::PointLight, 10> pointLights;
	};

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
			glm::mat4& modelMatrix = transComp.GetModelMatrix();

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
	}

	std::unique_ptr<VulkanBuffer>& MeshRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_VertRendererUBO;
		if (set == 2 && binding == 0) return m_TextureParamUBO;
		if (set == 2 && binding == 1) return m_DirectionalLightUBO;
		if (set == 2 && binding == 2) return m_PointLightUBO;
		

		__debugbreak();
		SPIECS_LOG("Out of Range");
	}
}