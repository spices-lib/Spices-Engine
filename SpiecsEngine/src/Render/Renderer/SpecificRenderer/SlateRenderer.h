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
		SlateRenderer(
			const std::string&                     rendererName          ,
			VulkanState&                           vulkanState           ,
			std::shared_ptr<VulkanDescriptorPool>  desctiptorPool        ,
			std::shared_ptr<VulkanDevice>          device                ,
			std::shared_ptr<RendererResourcePool>  rendererResourcePool
		)
			: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool, false) 
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateRenderer() { ShutdownImgui(); };

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

	private:

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific renderpass.
		*/
		virtual void CreateRendererPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific descriptorset for subpass.
		*/
		virtual void CreateDescriptorSet() override;

		/**
		* @brief Rewrite Renderer OnSystemInitialize to add other code.
		*/
		virtual void OnSystemInitialize() override;

		/**
		* @brief This interface is called on Window resized over (regist by swapchain).
		* If the specific renderer uses swpachianimage attachment during CreateRenderPass(),
		* this interface needs to override, callOnSlateResize here just will be fine.
		*/
		virtual void OnWindowResizeOver() override;

	private:

		/**
		* @brief Init Imgui.
		*/
		void InitImgui();

		/**
		* @brief Shutdown Imgui.
		*/
		void ShutdownImgui();

		/**
		* @brief Begin Imgui frame.
		*/
		void BeginImguiFrame();

		/**
		* @brief End Imgui frame.
		* @param[in] index CommandBuffer index.
		*/
		void EndImguiFrame(uint32_t index);
	};
}