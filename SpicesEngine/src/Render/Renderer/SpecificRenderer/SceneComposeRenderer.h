/**
* @file SceneComposeRenderer.h.
* @brief The SceneComposeRenderer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spices {

	/**
	* @brief SceneComposeRenderer Class.
	* This class defines how we compose the scene.
	*/
	class SceneComposeRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member variables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] descriptorPool The DescriptorPool.
		* @param[in] device The VulkanDevice, used for format query.
		* @param[in] rendererResourcePool The RendererResourcePool, RT Pool.
		*/
		SceneComposeRenderer(
			const std::string&                     rendererName          , 
			VulkanState&                           vulkanState           , 
			std::shared_ptr<VulkanDescriptorPool>  descriptorPool        , 
			std::shared_ptr<VulkanDevice>          device                , 
			std::shared_ptr<RendererResourcePool>  rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~SceneComposeRenderer() override = default;

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] ts TimeStep.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

	private:

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific render pass.
		*/
		virtual void CreateRendererPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific descriptor set for sub pass.
		*/
		virtual void CreateDescriptorSet() override;

	private:

		/**
		* @brief Full Screen Square for rendering.
		*/
		std::unique_ptr<SquarePack> m_Square;
	};
}