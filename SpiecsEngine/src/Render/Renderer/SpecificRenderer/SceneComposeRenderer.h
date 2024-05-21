/**
* @file SceneComposeRenderer.h.
* @brief The SceneComposeRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Render/Renderer/Renderer.h"


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
		SceneComposeRenderer(
			const std::string&                     rendererName          , 
			VulkanState&                           vulkanState           , 
			std::shared_ptr<VulkanDescriptorPool>  desctiptorPool        , 
			std::shared_ptr<VulkanDevice>          device                , 
			std::shared_ptr<RendererResourcePool>  rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~SceneComposeRenderer() {};

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

		virtual void CreateDescriptorSet() override;

		virtual void OnSlateResize() override;

	private:
		std::unique_ptr<SquarePack> m_Square;
	};
}