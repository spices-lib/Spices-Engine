#pragma once
#include "Renderer.h"

#include <glm/glm.hpp>

namespace Spiecs {

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool) 
			: Renderer(rendererName, vulkanState, desctiptorPool){};
		virtual ~MeshRenderer() {};

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
			std::unique_ptr<VulkanBuffer> m_VertUniformBuffer;

			// fragStage
			std::unique_ptr<VulkanImage> m_FragTexture;

		public:
			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) override;
			virtual std::unique_ptr<VulkanImage>& GetImage(uint32_t set, uint32_t binding) override;
		};
	};
}