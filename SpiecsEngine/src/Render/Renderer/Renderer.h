#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/Vulkan/VulkanBuffer.h"
#include "Render/Vulkan/VulkanImage.h"
#include "Render/FrameInfo.h"
#include "Render/Vulkan/VulkanDescriptor.h"
#include "RendererManager.h"

#include "World/Components/MeshComponent.h"
#include "World/Components/TransformComponent.h"
#include "World/Components/CameraComponent.h"
#include "World/Components/UUIDComponent.h"

#include <memory>
#include <unordered_map>

namespace Spiecs {

	class Renderer
	{
	public:
		Renderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool);
		virtual ~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		virtual void Render(FrameInfo& frameInfo) = 0;

		void OnSystemInitialize();

	private:
		// TODO: specific renderpass
		virtual void CreateRenderPass() = 0;

		// specific localdescriptor
		virtual void CreateLocalDescriptor() = 0;

		// specific desctiptor layout
		virtual void CreatePipelineLayout() = 0;

		// specific pipeline
		virtual void CreatePipeline(VkRenderPass renderPass) = 0;

	protected:
		std::string GetSahderPath(const std::string& shaderType);

		template<typename T, typename F>
		inline void IterWorldComp(FrameInfo& frameInfo, F func);

	private:
		struct DescriptorResource
		{
			std::vector<VkDescriptorSet> m_DescriptorSets{};
		};

	protected:
		// vulkan state
		VulkanState& m_VulkanState;
		std::shared_ptr<VulkanDescriptorPool> m_DesctiptorPool;

		// TODO: renderpass
		// VkRenderPass m_RenderPass;

		// descriptorset
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts{};
		std::array<DescriptorResource, MaxFrameInFlight> m_Resource{};

		// pipeline
		VkPipelineLayout m_PipelineLayout;
		std::unique_ptr<VulkanPipeline> m_VulkanPipeline;
		
		std::string m_RendererName;
	};

	template<typename T, typename F>
	inline void Renderer::IterWorldComp(FrameInfo& frameInfo, F func)
	{
		// group inter will be bug
		auto& view = frameInfo.m_World->GetRegistry().view<T>();
		for (auto& e : view)
		{
			auto& [tComp, transComp, uuidComp] = frameInfo.m_World->GetRegistry().get<T, TransformComponent, UUIDComponent>(e);

			bool isIterBreak = func(transComp, uuidComp.GetUUID(), tComp);

			if (isIterBreak) break;
		}
	}
}