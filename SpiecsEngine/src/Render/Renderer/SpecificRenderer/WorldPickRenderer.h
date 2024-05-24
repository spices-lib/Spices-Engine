/**
* @file WorldPickRenderer.h.
* @brief The WorldPickRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	/**
	* @brief WorldPickRenderer Class.
	* This class defines the picked entity render behaver.
	*/
	class WorldPickRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		WorldPickRenderer(
			const std::string&                     rendererName,
			VulkanState&                           vulkanState,
			std::shared_ptr<VulkanDescriptorPool>  desctiptorPool,
			std::shared_ptr<VulkanDevice>          device,
			std::shared_ptr<RendererResourcePool>  rendererResourcePool
		)
			: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool) 
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~WorldPickRenderer() {};

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
	};

}