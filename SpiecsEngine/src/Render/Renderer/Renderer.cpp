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

	inline Renderer::PipelineLayoutBuilder& Renderer::PipelineLayoutBuilder::AddTexture2D(uint32_t set, uint32_t binding, VkShaderStageFlags stageFlags)
	{
		// local data
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			ContainerLibrary::Resize<std::unique_ptr<VulkanImage>>(m_Renderer->m_Collections[i]->GetTexture(set, binding), 1);

			m_Renderer->m_Collections[i]->GetTexture(set, binding)[0] = std::make_unique<VulkanImage>(
				m_Renderer->m_VulkanState,
				SPIECS_ENGINE_ASSETS_PATH + "Textures/street.jpg"
			);
		}

		// descriptorset layout
		ContainerLibrary::Resize<std::unique_ptr<VulkanDescriptorSetLayout>>(m_VulkanLayouts, set + 1);

		m_VulkanLayouts[set] = VulkanDescriptorSetLayout::Builder()
			.AddBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, stageFlags)
			.Build(m_Renderer->m_VulkanState);

		ContainerLibrary::Resize<VkDescriptorSetLayout>(m_Renderer->m_DescriptorSetLayouts, set + 1);

		m_Renderer->m_DescriptorSetLayouts[set] = m_VulkanLayouts[set]->GetDescriptorSetLayout();

		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			ContainerLibrary::Resize<VkDescriptorSet>(m_Renderer->m_Resource[i].m_DescriptorSets, binding + 1);

			auto imageInfo = m_Renderer->m_Collections[i]->GetTexture(set, binding)[0]->GetImageInfo();
			VulkanDescriptorWriter(*m_VulkanLayouts[set], *m_Renderer->m_DesctiptorPool)
				.WriteImage(binding, &imageInfo)
				.Build(m_Renderer->m_Resource[i].m_DescriptorSets[binding]);
		}

		return *this;
	}

	void Renderer::PipelineLayoutBuilder::Build()
	{
		// create descriptor set
		for (int i = 0; i < MaxFrameInFlight; i++)
		{
			int setSize = m_VulkanLayoutWriters.size();
			ContainerLibrary::Resize<VkDescriptorSet>(m_Renderer->m_Resource[i].m_DescriptorSets, setSize);

			for (int j = 0; j < setSize; j++)
			{
				m_VulkanLayoutWriters[j]->Build(m_Renderer->m_Resource[i].m_DescriptorSets[j]);
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

}