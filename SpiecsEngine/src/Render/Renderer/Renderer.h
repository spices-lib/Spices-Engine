#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/FrameInfo.h"

#include <memory>
#include <unordered_map>

namespace Spiecs {

	class Renderer
	{
	public:
		Renderer(const std::string& rendererName, VulkanState& vulkanState);
		virtual ~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		virtual void InitDescriptor() {};
		virtual void Render(FrameInfo& frameInfo) {};

		virtual void OnSystemInitialize();

	private:
		virtual void CreatePipelineLayout() {};
		virtual void CreatePipeline(VkRenderPass renderPass) {};

	protected:
		std::string GetSahderPath(const std::string& shaderType);
	protected:
		VulkanState& m_VulkanState;


		VkPipelineLayout m_PipelineLayout;
		std::unique_ptr<VulkanPipeline> m_VulkanPipeline;
		
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<VkDescriptorSet> m_DescriptorSets;


		std::string m_RendererName;
	};

	class RendererManager
	{
	public:
		RendererManager() {};
		virtual ~RendererManager() {};

		static RendererManager& Get();
		static void Run(FrameInfo& frameInfo);

		template<typename T, typename ... Args>
		RendererManager& Push(const std::string& rendererName, Args&& ... args)
		{
			// push system to map
			if (m_Identities.find(rendererName) != m_Identities.end())
			{
				std::cout << "ERROR: " << rendererName << " has been pushed" << std::endl;
				__debugbreak();
			}

			m_Identities[rendererName] = std::unique_ptr<Renderer>(reinterpret_cast<Renderer*>(new T(rendererName, std::forward<Args>(args)...)));
			m_Identities[rendererName]->OnSystemInitialize();

			// system init
			std::cout << "INFO: " << rendererName << " pushed" << std::endl;

			return *m_RendererManager;
		}

		RendererManager& Pop(const std::string& rendererName)
		{
			// pop system from map
			if (m_Identities.find(rendererName) == m_Identities.end())
			{
				std::cout << "ERROR: " << rendererName << " has been poped" << std::endl;
				__debugbreak();
			}

			// system shutdown
			std::cout << "INFO: " << rendererName << " poped" << std::endl;

			m_Identities.erase(rendererName);

			return *m_RendererManager;
		}

	private:
		static std::unique_ptr<RendererManager> m_RendererManager;
		static std::unordered_map<std::string, std::unique_ptr<Renderer>> m_Identities;
	};
}