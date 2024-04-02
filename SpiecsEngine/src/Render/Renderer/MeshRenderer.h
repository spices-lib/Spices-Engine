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
		virtual void InitUniformBuffer() override;
		virtual void InitDescriptor() override;
		virtual void CreatePipelineLayout() override;
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	private:
		std::pair<glm::mat4, glm::mat4> GetActiveCameraMatrix(FrameInfo& frameInfo);

	private:
		// uniformbuffer
		std::vector<std::unique_ptr<VulkanBuffer>> m_UniformBuffers;
	};
}