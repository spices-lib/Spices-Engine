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
		CreateLocalDescriptor();
		CreatePipelineLayout();
		CreatePipeline(m_VulkanState.m_RenderPass);
	}

	std::string Renderer::GetSahderPath(const std::string& shaderType)
	{
		return SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/Shader." + m_RendererName + "." + shaderType + ".spv";
	}
}