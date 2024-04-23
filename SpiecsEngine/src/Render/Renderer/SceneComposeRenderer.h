/**
* @file SceneComposeRenderer.h.
* @brief The SceneComposeRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Renderer.h"


namespace Spiecs {

	/**
	* @brief SceneComposeRenderer Class.
	* This class defines how we compose the scene.
	*/
	class SceneComposeRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		SceneComposeRenderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool, std::shared_ptr<VulkanDevice> device, std::shared_ptr<RendererResourcePool> rendererResourcePool)
			: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~SceneComposeRenderer() {};

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(FrameInfo& frameInfo) override;

	private:

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific renderpass.
		*/
		virtual void CreateRenderPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific pipelinelayout and buffer type descriptor.
		*/
		virtual void CreatePipelineLayoutAndDescriptor() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific pipeline.
		* @param[in] renderPass Renderer specific renderpass.
		*/
		virtual void CreatePipeline(VkRenderPass renderPass) override;
	};
}