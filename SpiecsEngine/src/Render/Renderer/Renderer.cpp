#include "pchheader.h"
#include "Renderer.h"

namespace Spiecs {

	Renderer::Renderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool)
		: m_RendererName(rendererName)
		, m_VulkanState(vulkanState) 
		, m_DesctiptorPool(desctiptorPool)
	{}

	Renderer::~Renderer()
	{
		vkDestroyPipelineLayout(m_VulkanState.m_Device, m_PipelineLayout, nullptr);
	}

	void Renderer::OnSystemInitialize()
	{
		CreateRenderPass();


		CreatePipelineLayoutAndDescriptor();


		CreatePipeline(m_VulkanState.m_RenderPass);
	}

	std::string Renderer::GetSahderPath(const std::string& shaderType)
	{
		return SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/Shader." + m_RendererName + "." + shaderType + ".spv";
	}

	std::pair<glm::mat4, glm::mat4> Renderer::GetActiveCameraMatrix(FrameInfo& frameInfo)
	{
		glm::mat4 viewMat = glm::mat4(1.0f);
		glm::mat4 projectionMat = glm::mat4(1.0f);

		IterWorldComp<CameraComponent>(frameInfo, [&](TransformComponent& transComp, CameraComponent& camComp) {
			if (camComp.IsActived())
			{
				viewMat = glm::inverse(transComp.GetModelMatrix());
				viewMat[1][1] *= -1; // in vulkan, we need reverse y axis;
				projectionMat = camComp.GetCamera()->GetPMatrix();
				return true;
			}
			return false;
		});

		return std::make_pair(viewMat, projectionMat);
	}

	void Renderer::PipelineLayoutBuilder::Build()
	{
		// create descriptor set
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			int setSize = m_Renderer->m_VulkanLayoutWriters.size();
			ContainerLibrary::Resize<VkDescriptorSet>(m_Renderer->m_Resource[i].m_DescriptorSets, setSize);

			for (int j = 0; j < setSize; j++)
			{
				m_Renderer->m_VulkanLayoutWriters[j]->Build(m_Renderer->m_Resource[i].m_DescriptorSets[j]);
			}
		}

		// create pipelinelayout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_Renderer->m_DescriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = m_Renderer->m_DescriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (isUsePushConstant)
		{
			pipelineLayoutInfo.pushConstantRangeCount = 1;
			pipelineLayoutInfo.pPushConstantRanges = &m_PushConstantRange;
		}

		VK_CHECK(vkCreatePipelineLayout(m_Renderer->m_VulkanState.m_Device, &pipelineLayoutInfo, nullptr, &m_Renderer->m_PipelineLayout));
	}

	Renderer::RenderBehaverBuilder::RenderBehaverBuilder(Renderer* renderer, uint32_t currentFrame)
		: m_Renderer(renderer)
		, m_CurrentFrame(currentFrame)
	{
		// bind pipeline
		BindPipeline();

		// bind descriptorsets all sets
		BindAllBufferTyepDescriptorSet();
	}

	void Renderer::RenderBehaverBuilder::BindPipeline()
	{
		m_Renderer->m_VulkanPipeline->Bind(m_CurrentFrame);
	}

	void Renderer::RenderBehaverBuilder::BindAllBufferTyepDescriptorSet()
	{
		int setCount = m_Renderer->m_VulkanLayoutWriters.size();
		for (int i = 0; i < setCount; i++)
		{
			bool IsPureBufferTypeSet = true;

			int bindingCount = m_Renderer->m_VulkanLayoutWriters[i]->GetWritters().size();
			for (int j = 0; j < bindingCount; j++)
			{
				if (m_Renderer->m_VulkanLayoutWriters[i]->GetWritters()[j].descriptorType != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				{
					IsPureBufferTypeSet = false;
					break;
				}
			}

			if (IsPureBufferTypeSet)
			{
				BindDescriptorSet(i, m_Renderer->m_Resource[m_CurrentFrame].m_DescriptorSets[i]);
			}
		}
	}

	void Renderer::RenderBehaverBuilder::BindDescriptorSet(uint32_t set, VkDescriptorSet& descriptorset)
	{
		vkCmdBindDescriptorSets(
			m_Renderer->m_VulkanState.m_CommandBuffer[m_CurrentFrame],
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_Renderer->m_PipelineLayout,
			set,
			1,
			&descriptorset,
			0,
			nullptr
		);
	}
}