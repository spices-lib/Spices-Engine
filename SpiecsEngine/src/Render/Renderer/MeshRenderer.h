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
		std::pair<glm::mat4, glm::mat4> GetActiveCameraMatrix(FrameInfo& frameInfo);

	private:
		struct SpecificCollection : public Collection
		{
		private:
			// vertexStage
			std::unique_ptr<VulkanBuffer> m_VertUniformBuffer;

			// fragStage
			std::vector<std::unique_ptr<VulkanImage>> m_FragTexture2Ds;
			std::unique_ptr<VulkanBuffer> m_FragUniformBuffer0;
			std::unique_ptr<VulkanBuffer> m_FragUniformBuffer;

		public:
			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) override;
			virtual std::vector<std::unique_ptr<VulkanImage>>& GetTexture(uint32_t set, uint32_t binding) override;
		};
	};
}