/**
* @file ShadowRenderer.h.
* @brief The ShadowRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	/**
	* @brief ShadowRenderer Class.
	* This class defines the shadow render behaver.
	*/
	class ShadowRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		ShadowRenderer(
			const std::string&                     rendererName         ,
			VulkanState&                           vulkanState          ,
			std::shared_ptr<VulkanDescriptorPool>  desctiptorPool       ,
			std::shared_ptr<VulkanDevice>          device               ,
			std::shared_ptr<RendererResourcePool>  rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~ShadowRenderer() {};

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

	private:

		/**
		* @brief Full Screen Square for rendering.
		*/
		std::unique_ptr<SquarePack> m_Square;
	};
}