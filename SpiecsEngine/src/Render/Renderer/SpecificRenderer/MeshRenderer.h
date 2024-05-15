/**
* @file MeshRenderer.h.
* @brief The MeshRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Render/Renderer/Renderer.h"
#include <glm/glm.hpp>

namespace Spiecs {

	/**
	* @brief MeshRenderer Class.
	* This class defines the mesh component render behaver.
	*/
	class MeshRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		MeshRenderer(
			const std::string&                     rendererName, 
			VulkanState&                           vulkanState, 
			std::shared_ptr<VulkanDescriptorPool> desctiptorPool, 
			std::shared_ptr<VulkanDevice>         device, 
			std::shared_ptr<RendererResourcePool> rendererResourcePool
		)
			: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool){};

		/**
		* @brief Destructor Function.
		*/
		virtual ~MeshRenderer() {};

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
		virtual void CreateRenderPass() override;

		virtual void CreateDescriptorSet() override;

		virtual void OnSlateResize() override { CreateRenderPass(); };
	};
}