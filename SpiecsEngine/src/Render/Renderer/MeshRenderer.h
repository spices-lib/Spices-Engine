#pragma once
#include "Renderer.h"

#include <glm/glm.hpp>

namespace Spiecs {

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(const std::string& rendererName, VulkanState& vulkanState) : Renderer(rendererName, vulkanState){};
		virtual ~MeshRenderer() {};

		virtual void InitDescriptor() override;
		virtual void Render(FrameInfo& frameInfo) override;

	private:
		virtual void CreatePipelineLayout() override;
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	private:
		std::pair<glm::mat4, glm::mat4> GetActiveCameraMatrix(FrameInfo& frameInfo);
	};
}