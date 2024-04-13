#pragma once
#include "Core/Core.h"
#include "Renderer.h"

namespace Spiecs {

	class SkyBoxRenderer : public Renderer
	{
	public:
		SkyBoxRenderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool)
			: Renderer(rendererName, vulkanState, desctiptorPool) {};
		virtual ~SkyBoxRenderer() {};

		virtual void Render(FrameInfo& frameInfo) override;

	private:
		virtual void CreateRenderPass() override {};
		virtual void CreatePipelineLayoutAndDescriptor() override;
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	private:

	private:
		struct SpecificCollection : public Collection
		{
		private:
			// vertexStage
			std::unique_ptr<VulkanBuffer> m_VertRendererUBO;

		public:
			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) override;
		};
	};

}