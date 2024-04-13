#include "Pchheader.h"
#include "SkyBoxRenderer.h"

namespace Spiecs {

	struct PushConstant
	{
		glm::mat4 model = glm::mat4(1.0f);
		int entityID = -1;
		int meshpackID = -1;
	};

	struct VertRendererUBO
	{
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	void SkyBoxRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.CreateCollection<SpecificCollection>()
		.AddPushConstant<PushConstant>()
		.AddBuffer<VertRendererUBO>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
		.AddTexture<Texture2D>(1, 0, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
		.Build();
	}

	void SkyBoxRenderer::CreatePipeline(VkRenderPass renderPass)
	{
		PipelineConfigInfo pipelineConfig{};
		VulkanPipeline::DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		m_VulkanPipeline = std::make_unique<VulkanPipeline>(
			m_VulkanState,
			GetSahderPath("vert"),
			GetSahderPath("frag"),
			pipelineConfig
		);
	}

	void SkyBoxRenderer::Render(FrameInfo& frameInfo)
	{
		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex };

		builder.UpdateBuffer<VertRendererUBO>(0, 0, [&](auto& ubo) {
			auto& [viewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);
			ubo.view = viewMatrix;
			ubo.projection = projectionMatrix;
			});

		IterWorldComp<SkyBoxComponent>(frameInfo, [&](int entityId, TransformComponent& transComp, SkyBoxComponent& skyboxComp) {
			glm::mat4& modelMatrix = transComp.GetModelMatrix();

			skyboxComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](uint32_t meshpackId, auto material) {
				builder.UpdatePushConstant<PushConstant>([&](auto& push) {
					push.model = modelMatrix;
					push.entityID = entityId;
					push.meshpackID = meshpackId;
					});

				builder.BindDescriptorSet(1, material->GetMaterialDescriptorSet());
				});

			return false;
		});
	}

	std::unique_ptr<VulkanBuffer>& SkyBoxRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_VertRendererUBO;
	}
}