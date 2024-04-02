#include "pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "World/World/World.h"

namespace Spiecs {

	struct MeshRendererPushConstant
	{
		glm::mat4 model;
	};

	struct MeshRendererUniformBuffer
	{
		glm::mat4 view;
		glm::mat4 projection;
	};

	void MeshRenderer::InitUniformBuffer()
	{
		m_UniformBuffers.resize(MaxFrameInFlight);
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_UniformBuffers[i] = std::make_unique<VulkanBuffer>(
				m_VulkanState,
				sizeof(MeshRendererUniformBuffer),
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			m_UniformBuffers[i]->Map();
		}
	}

	void MeshRenderer::Render(FrameInfo& frameInfo)
	{
		m_VulkanPipeline->Bind(frameInfo.m_FrameIndex);

		vkCmdBindDescriptorSets(
			m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0,
			1,
			&m_DescriptorSets[frameInfo.m_FrameIndex],
			0,
			nullptr
		);

		auto& [viewMatrix, projectionMatrix] =  GetActiveCameraMatrix(frameInfo);

		IterWorldComp<MeshComponent>(frameInfo, [&](TransformComponent& transComp, uint64_t uuid, MeshComponent& meshComp) {
			glm::mat4& modelMatrix = transComp.GetMMatrix();

			MeshRendererPushConstant push{};
			push.model = modelMatrix;

			vkCmdPushConstants(
				m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex],
				m_PipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(MeshRendererPushConstant),
				&push
			);

			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);

			return false;
		});
	}

	void MeshRenderer::CreatePipelineLayout()
	{
		auto descriptorSetLayout = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.Build(m_VulkanState);

		m_DescriptorSetLayouts = { descriptorSetLayout->GetDescriptorSetLayout() };

		m_DescriptorSets.resize(MaxFrameInFlight);
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			auto bufferInfo = m_UniformBuffers[i]->GetBufferInfo();
			VulkanDescriptorWriter(*descriptorSetLayout, *m_DesctiptorPool)
				.WriteBuffer(0, &bufferInfo)
				.Build(m_DescriptorSets[i]);
		}

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(MeshRendererPushConstant);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_DescriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = m_DescriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		VK_CHECK(vkCreatePipelineLayout(m_VulkanState.m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout));
	}

	void MeshRenderer::CreatePipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

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

	std::pair<glm::mat4, glm::mat4> MeshRenderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 projectionMat = glm::mat4(1.0f);

		IterWorldComp<CameraComponent>(frameInfo, [&](TransformComponent& transComp, uint64_t uuid, CameraComponent& camComp) {
			if (camComp.IsActived())
			{
				viewMat = transComp.GetVMatrix();
				projectionMat = camComp.GetCamera()->GetPMatrix();
				return true;
			}
		});

		return std::make_pair(viewMat, projectionMat);
	}

}