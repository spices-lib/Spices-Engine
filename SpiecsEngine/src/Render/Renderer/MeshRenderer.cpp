#include "pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "World/World/World.h"

namespace Spiecs {

	void MeshRenderer::InitDescriptor()
	{
		auto descriptorSetLayout = VulkanDescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.Build(m_VulkanState);

		std::vector<VkDescriptorSet> descriptorSet(MaxFrameInFlight);
		/*for (int i = 0; i < descriptorSet.size(); i++)
		{
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			VulkanDescriptorWriter(*descriptorSetLayout, *globalPool)
				.WriteBuffer(0, &bufferInfo)
				.Build(descriptorSet[i]);
		}*/
	}

	void MeshRenderer::Render(FrameInfo& frameInfo)
	{
		m_VulkanPipeline->Bind(frameInfo.m_FrameIndex);

		/*vkCmdBindDescriptorSets(
			m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0,
			static_cast<uint32_t>(m_DescriptorSets.size()),
			m_DescriptorSets.data(),
			0,
			nullptr
		);*/

		auto& [viewMatrix, projectionMatrix] =  GetActiveCameraMatrix(frameInfo);

		IterWorldComp<MeshComponent>(frameInfo, [&](TransformComponent& transComp, uint64_t uuid, MeshComponent& meshComp) {
			glm::mat4& modelMatrix = transComp.GetMMatrix();
			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
			return false;
		});
	}

	void MeshRenderer::CreatePipelineLayout()
	{
		/*VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);*/

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_DescriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = m_DescriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

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