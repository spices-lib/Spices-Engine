#pragma once
#include "Renderer.h"

namespace Spiecs {

	class MeshRenderer : public Renderer
	{
	public:
		MeshRenderer(VulkanState& vulkanState) : Renderer(vulkanState){};
		virtual ~MeshRenderer() {};

		MeshRenderer(const MeshRenderer&) = delete;
		MeshRenderer& operator=(const MeshRenderer&) = delete;

		virtual void InitDescriptor() override;
		virtual void Render(FrameInfo& frameInfo) override;

	private:
		virtual void CreatePipelineLayout() override;
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	};
}