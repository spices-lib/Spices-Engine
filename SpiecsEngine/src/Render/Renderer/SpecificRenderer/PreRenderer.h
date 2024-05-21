#pragma once

#include "Core/Core.h"
#include "Core/Math/Math.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	namespace PreR {

		/**
		* @brief This struct is specific SkyBoxRenderer PsuhConstant
		*/
		struct PushConstant
		{
			/**
			* @brief Meshpack ModelMatrix.
			*/
			glm::mat4 model = glm::mat4(1.0f);

			/**
			* @brief Entityid, cast from entt::entity.
			*/
			int entityID = -1;
		};

		/**
		* @breif Global View struct.
		*/
		struct View
		{
			/**
			* @brief Projection Matrix.
			*/
			glm::mat4 projection = glm::mat4(1.0f);

			/**
			* @brief View Matrix.
			*/
			glm::mat4 view = glm::mat4(1.0f);

			/**
			* @brief inverse of View Matrix.
			*/
			glm::mat4 inView = glm::mat4(1.0f);

			/**
			* @brief SceneTexturesize(ViewportSize) in component xy, 1.0f / SceneTexturesize in component zw.
			*/
			glm::vec4 sceneTextureSize = glm::vec4(1.0f);

			/**
			* @brief WindowSize(ApplicationSize) in component xy, 1.0f / WindowSize in component zw.
			*/
			glm::vec4 windowSize = glm::vec4(1.0f);
		};

		/**
		* @breif Global Input struct.
		*/
		struct SpiecsInput
		{
			/**
			* @brief Viewport's MousePos in component xy, 1.0f / MousePos in component zw.
			*/
			glm::vec4 mousePos = glm::vec4(1.0f);

			/**
			* @brief Game's time.
			*/
			float gameTime = 0.0f;

			/**
			* @brief Frame's time.
			*/
			float frameTime = 0.0f;
		};

	}

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
            const std::string&                     rendererName,
            VulkanState&                           vulkanState,
            std::shared_ptr<VulkanDescriptorPool>  desctiptorPool,
            std::shared_ptr<VulkanDevice>          device,
            std::shared_ptr<RendererResourcePool>  rendererResourcePool
        )
            : Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool, false)
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
        virtual void CreateRendererPass() override {};

        virtual void CreateDescriptorSet() override;
        
    };
}