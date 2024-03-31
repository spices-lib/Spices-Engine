#include "pchheader.h"
#include "Renderer.h"

namespace Spiecs {

	Renderer::Renderer(const std::string& rendererName, VulkanState& vulkanState)
		: m_RendererName(rendererName)
		, m_VulkanState(vulkanState) 
	{}

	Renderer::~Renderer()
	{
		vkDestroyPipelineLayout(m_VulkanState.m_Device, m_PipelineLayout, nullptr);
	}

	void Renderer::OnSystemInitialize()
	{
		InitDescriptor();
		CreatePipelineLayout();
		CreatePipeline(m_VulkanState.m_RenderPass);
	}

	std::string Renderer::GetSahderPath(const std::string& shaderType)
	{
		return SPIECS_ENGINE_ASSETS_PATH + "Shaders/spv/Shader." + m_RendererName + "." + shaderType + ".spv";
	}

	std::unique_ptr<RendererManager> RendererManager::m_RendererManager;
	std::unordered_map<std::string, std::unique_ptr<Renderer>> RendererManager::m_Identities;

	RendererManager& RendererManager::Get()
	{
	    return *m_RendererManager;
	}

	void RendererManager::Run(FrameInfo& frameInfo)
	{
		for (auto& pair : m_Identities)
		{
			pair.second->Render(frameInfo);
		}
	}
}