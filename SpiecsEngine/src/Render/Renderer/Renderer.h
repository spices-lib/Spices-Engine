#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanPipeline.h"
#include "Render/Vulkan/VulkanUtils.h"
#include "Render/FrameInfo.h"

#include <memory>

namespace Spiecs {

	class Renderer
	{
	public:
		Renderer(VulkanState& vulkanState);
		virtual ~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		virtual void InitDescriptor() {};
		virtual void Render(FrameInfo& frameInfo) {};

	private:
		virtual void CreatePipelineLayout() {};
		virtual void CreatePipeline(VkRenderPass renderPass) {};

	protected:
		VulkanState& m_VulkanState;

		VkPipelineLayout m_PipelineLayout;
		std::unique_ptr<VulkanPipeline> m_VulkanPipeline;
		
		std::vector<VkDescriptorSetLayout> m_DescriptorSetLayouts;
		std::vector<VkDescriptorSet> m_DescriptorSets;

	};
}