#include "pchheader.h"
#include "Renderer.h"

namespace Spiecs {

	Renderer::Renderer(VulkanState& vulkanState)
		: m_VulkanState(vulkanState)
	{
		InitDescriptor();
		CreatePipelineLayout();
		CreatePipeline(vulkanState.m_RenderPass);
	}

	Renderer::~Renderer()
	{
		vkDestroyPipelineLayout(m_VulkanState.m_Device, m_PipelineLayout, nullptr);
	}
}