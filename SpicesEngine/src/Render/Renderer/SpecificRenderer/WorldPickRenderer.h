/**
* @file WorldPickRenderer.h.
* @brief The WorldPickRenderer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spices {

	/**
	* @brief WorldPickRenderer Class.
	* This class defines the picked entity render behaves.
	*/
	class WorldPickRenderer : public Renderer
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
		WorldPickRenderer(
			const std::string&                           rendererName         ,
			VulkanState&                                 vulkanState          ,
			const std::shared_ptr<VulkanDescriptorPool>& descriptorPool       ,
			const std::shared_ptr<VulkanDevice>&         device               ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool
		)
			: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool) 
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~WorldPickRenderer() override = default;

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
	};

}