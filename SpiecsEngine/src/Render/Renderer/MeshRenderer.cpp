#include "pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "World/World/World.h"

namespace Spiecs {

	struct PushConstant
	{
		glm::mat4 model = glm::mat4(1.0f);
	};

	struct VertUniformBuffer
	{
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	struct FragUniformBuffer
	{
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	void MeshRenderer::CreatePipelineLayoutAndDescriptor()
	{
		PipelineLayoutBuilder{ this }
		.CreateCollection<SpecificCollection>()
		.AddPushConstant<PushConstant>()
		.AddBuffer<VertUniformBuffer>(0, 0, VK_SHADER_STAGE_VERTEX_BIT)
		.AddBuffer<FragUniformBuffer>(1, 0, VK_SHADER_STAGE_FRAGMENT_BIT)
		.AddBuffer<FragUniformBuffer>(1, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
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
		m_VulkanPipeline->Bind(frameInfo.m_FrameIndex);

		vkCmdBindDescriptorSets(
			m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0,
			m_Resource[0].m_DescriptorSets.size(),
			m_Resource[frameInfo.m_FrameIndex].m_DescriptorSets.data(),
			0,
			nullptr
		);

		auto& [viewMatrix, projectionMatrix] = GetActiveCameraMatrix(frameInfo);

		VertUniformBuffer vertubo{};
		vertubo.view = viewMatrix;
		vertubo.projection = projectionMatrix;
		m_Collections[frameInfo.m_FrameIndex]->GetBuffer(0, 0)->WriteToBuffer(&vertubo);
		m_Collections[frameInfo.m_FrameIndex]->GetBuffer(0, 0)->Flush();

		FragUniformBuffer fragubo{};
		fragubo.view = projectionMatrix;
		fragubo.projection = viewMatrix;
		m_Collections[frameInfo.m_FrameIndex]->GetBuffer(1, 0)->WriteToBuffer(&fragubo);
		m_Collections[frameInfo.m_FrameIndex]->GetBuffer(1, 0)->Flush();

		m_Collections[frameInfo.m_FrameIndex]->GetBuffer(1, 1)->WriteToBuffer(&fragubo);
		m_Collections[frameInfo.m_FrameIndex]->GetBuffer(1, 1)->Flush();

		IterWorldComp<MeshComponent>(frameInfo, [&](TransformComponent& transComp, MeshComponent& meshComp) {
			glm::mat4& modelMatrix = transComp.GetModelMatrix();

			PushConstant push{};
			push.model = modelMatrix;

			vkCmdPushConstants(
				m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex],
				m_PipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstant),
				&push
			);

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

			return false;
		});
	}

	std::pair<glm::mat4, glm::mat4> MeshRenderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 projectionMat = glm::mat4(1.0f);

		IterWorldComp<CameraComponent>(frameInfo, [&](TransformComponent& transComp, CameraComponent& camComp) {
			if (camComp.IsActived())
			{
				viewMat = glm::inverse(transComp.GetModelMatrix());
				projectionMat = camComp.GetCamera()->GetPMatrix();
				return true;
			}
		});

		return std::make_pair(viewMat, projectionMat);
	}

	std::unique_ptr<VulkanBuffer>& MeshRenderer::SpecificCollection::GetBuffer(uint32_t set, uint32_t binding)
	{
		if (set == 0 && binding == 0) return m_VertUniformBuffer;
		if (set == 1 && binding == 0) return m_FragUniformBuffer0;
		if (set == 1 && binding == 1) return m_FragUniformBuffer;

		__debugbreak();
		SPIECS_LOG("Out of Range");
	}

	std::vector<std::unique_ptr<VulkanImage>>& MeshRenderer::SpecificCollection::GetTexture(uint32_t set, uint32_t binding)
	{
		if (set == 1 && binding == 0) return m_FragTexture2Ds;

		__debugbreak();
		SPIECS_LOG("Out of Range");
	}
}