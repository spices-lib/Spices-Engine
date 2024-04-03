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

	void MeshRenderer::CreateLocalDescriptor()
	{
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_Collections[i].m_VertUniformBuffer = std::make_unique<VulkanBuffer>(
				m_VulkanState,
				sizeof(VertUniformBuffer),
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			m_Collections[i].m_VertUniformBuffer->Map();

			m_Collections[i].m_FragUniformBuffer = std::make_unique<VulkanBuffer>(
				m_VulkanState,
				sizeof(FragUniformBuffer),
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);
			m_Collections[i].m_FragUniformBuffer->Map();
		}
	}

	void MeshRenderer::CreatePipelineLayout()
	{
		auto vertDescriptorSetLayout = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.Build(m_VulkanState);

		auto fragDescriptorSetLayout = VulkanDescriptorSetLayout::Builder()
			.AddBinding(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.Build(m_VulkanState);

		m_DescriptorSetLayouts = { 
			vertDescriptorSetLayout->GetDescriptorSetLayout(), 
			fragDescriptorSetLayout->GetDescriptorSetLayout() 
		};

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			m_Resource[i].m_DescriptorSets.resize(2);

			auto vertBufferInfo = m_Collections[i].m_VertUniformBuffer->GetBufferInfo();
			VulkanDescriptorWriter(*vertDescriptorSetLayout, *m_DesctiptorPool)
				.WriteBuffer(0, &vertBufferInfo)
				.Build(m_Resource[i].m_DescriptorSets[0]);

			auto fragBufferInfo = m_Collections[i].m_FragUniformBuffer->GetBufferInfo();
			VulkanDescriptorWriter(*fragDescriptorSetLayout, *m_DesctiptorPool)
				.WriteBuffer(1, &fragBufferInfo)
				.Build(m_Resource[i].m_DescriptorSets[1]);
		}

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstant);

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
		m_Collections[frameInfo.m_FrameIndex].m_VertUniformBuffer->WriteToBuffer(&vertubo);
		m_Collections[frameInfo.m_FrameIndex].m_VertUniformBuffer->Flush();

		FragUniformBuffer fragubo{};
		fragubo.view = projectionMatrix;
		fragubo.projection = viewMatrix;
		m_Collections[frameInfo.m_FrameIndex].m_FragUniformBuffer->WriteToBuffer(&fragubo);
		m_Collections[frameInfo.m_FrameIndex].m_FragUniformBuffer->Flush();

		IterWorldComp<MeshComponent>(frameInfo, [&](TransformComponent& transComp, uint64_t uuid, MeshComponent& meshComp) {
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

		IterWorldComp<CameraComponent>(frameInfo, [&](TransformComponent& transComp, uint64_t uuid, CameraComponent& camComp) {
			if (camComp.IsActived())
			{
				viewMat = glm::inverse(transComp.GetModelMatrix());
				projectionMat = camComp.GetCamera()->GetPMatrix();
				return true;
			}
		});

		return std::make_pair(viewMat, projectionMat);
	}
}