#pragma once
#include "Renderer.h"

#include <glm/glm.hpp>

namespace Spiecs {

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool) 
			: Renderer(rendererName, vulkanState, desctiptorPool){};

		virtual void Render(FrameInfo& frameInfo) override;

	private:
		virtual void CreateRenderPass() override {};
		virtual void CreateLocalDescriptor() override;
		virtual void CreatePipelineLayout() override;
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	private:
		std::pair<glm::mat4, glm::mat4> GetActiveCameraMatrix(FrameInfo& frameInfo);

	private:
		struct MeshRendererDescriptorSetCollection
		{
			// vertexStage
			std::unique_ptr<VulkanBuffer> m_VertUniformBuffer;

			// fragStage
			std::unique_ptr<VulkanBuffer> m_FragUniformBuffer;
		};

		std::array<MeshRendererDescriptorSetCollection, MaxFrameInFlight> m_Collections{};
	};
}