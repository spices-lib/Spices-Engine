#pragma once

#include "Core/Core.h"
#include "Core/Math/Math.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

    class PreRenderer : public Renderer
    {
    public:

        /**
        * @brief Constructor Function.
        * Init member veriables.
        * @param[in] rendererName The name of this Renderer.
        * @param[in] vulkanState The core vulkan objects that in use.
        * @param[in] desctiptorPool The DesctiptorPool.
        */
        PreRenderer(
            const std::string& rendererName,
            VulkanState& vulkanState,
            std::shared_ptr<VulkanDescriptorPool> desctiptorPool,
            std::shared_ptr<VulkanDevice>         device,
            std::shared_ptr<RendererResourcePool> rendererResourcePool
        )
            : Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
        {};

        /**
         * @brief Destructor Function.
         */
        virtual ~PreRenderer() {};

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
        virtual void CreateRenderPass() override {};

        virtual void CreateDescriptorSet() override;

    private:

        /**
        * @brief m_ConstantParams's VkBuffer.
        * Key: set, Value: VkBuffer.
        */
        std::unordered_map<Int2, std::unique_ptr<VulkanBuffer>> m_Buffers;
    };
}