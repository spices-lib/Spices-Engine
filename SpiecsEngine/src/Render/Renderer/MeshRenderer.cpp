#include "pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"

namespace Spiecs {

	struct PushConstant
	{
		glm::mat4 model = glm::mat4(1.0f);
	};

	struct VertRendererUBO
	{
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	void MeshRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.CreateCollection<SpecificCollection>()
		.AddPushConstant<PushConstant>()
		.AddBuffer<VertRendererUBO>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
		.AddTexture<Texture2D>(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
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

		auto& [viewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);

		builder.UpdateBuffer<VertRendererUBO>(0, 0, [&](auto& ubo) {
			ubo.view = viewMatrix;
			ubo.projection = projectionMatrix;
		});

		IterWorldComp<MeshComponent>(frameInfo, [&](TransformComponent& transComp, MeshComponent& meshComp) {
			glm::mat4& modelMatrix = transComp.GetModelMatrix();

			// bind push constant
			builder.UpdatePushConstant<PushConstant>([&](auto& push) {
				push.model = modelMatrix;
			});

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], [&](auto material) {
				builder.BindDescriptorSet(1, material->GetTextureDescriptorSet(1, 0));
			});

			return false;
		});
	}

	std::unique_ptr<VulkanBuffer>& MeshRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_VertRendererUBO;

		__debugbreak();
		SPIECS_LOG("Out of Range");
	}

	std::unique_ptr<VulkanImage>& MeshRenderer::SpecificCollection::GetImage(uint32_t set, uint32_t binding)
	{
		if (set == 1 && binding == 0) return m_FragTexture;

		__debugbreak();
		SPIECS_LOG("Out of Range");
	}
}