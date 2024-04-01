#include "pchheader.h"
#include "MeshRenderer.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "World/World/World.h"
#include "World/Components/MeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/UUIDComponent.h"

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

		/*for (int i = 0; i < frameInfo.m_Meshes.size(); i++)
		{
			frameInfo.m_Meshes[i]->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		}*/

		auto group = frameInfo.m_World->GetRegistry().group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto [transformComp, meshComp] = group.get<TransformComponent, MeshComponent>(entity);
			meshComp.GetMesh()->Draw(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex]);
		}
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

}