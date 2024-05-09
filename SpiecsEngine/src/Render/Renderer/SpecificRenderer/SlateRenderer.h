/**
* @file SlateRenderer.h.
* @brief The SlateRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	/**
	* @brief SlateRenderer Class.
	* This class defines the imgui render behaver.
	*/
	class SlateRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		SlateRenderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool, std::shared_ptr<VulkanDevice> device, std::shared_ptr<RendererResourcePool> rendererResourcePool)
			: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateRenderer() { ShutdownImgui(); };

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
		virtual void CreatePipeline(VkRenderPass renderPass) override {};

		virtual void OnWindowResizeOver() override { CreateRenderPass(); };
		virtual void OnSlateResize() override { CreateRenderPass(); };

		virtual void OnSystemInitialize() override;

		void InitImgui();
		void ShutdownImgui();
		void BeginImguiFrame();
		void EndImguiFrame(uint32_t index);

	private:
	};

}