/**
* @file PreRenderer.h.
* @brief The PreRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once

#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	/**
	* @brief PreRenderer Class.
	* This class defines the global descriptor set.
	*/
    class PreRenderer : public Renderer
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
        PreRenderer(
            const std::string&                     rendererName,
            VulkanState&                           vulkanState,
            std::shared_ptr<VulkanDescriptorPool>  descriptorPool,
            std::shared_ptr<VulkanDevice>          device,
            std::shared_ptr<RendererResourcePool>  rendererResourcePool
        )
            : Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, false)
        {}

        /**
         * @brief Destructor Function.
         */
        virtual ~PreRenderer() override = default;

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