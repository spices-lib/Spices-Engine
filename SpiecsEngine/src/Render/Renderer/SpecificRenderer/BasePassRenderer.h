/**
* @file BasePassRenderer.h.
* @brief The BasePassRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	/**
	* @brief BasePassRenderer Class.
	* This class defines the base pass render behaver.
	*/
	class BasePassRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		BasePassRenderer(
			const std::string&                    rendererName        ,
			VulkanState&                          vulkanState         ,
			std::shared_ptr<VulkanDescriptorPool> desctiptorPool      ,
			std::shared_ptr<VulkanDevice>         device              ,
			std::shared_ptr<RendererResourcePool> rendererResourcePool
		)
			: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
		{};

		/**
		* @brief Destructor Function.
		*/
		virtual ~BasePassRenderer() {};

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

	private:
		 
		/**
		* @brief The interface is inherited from Renderer.
		* Create Specific Rendererpass.
		*/
		virtual void CreateRendererPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific descriptorset for subpass.
		*/
		virtual void CreateDescriptorSet() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create Material Specific Pipeline.
		* @todo Complete it by renderer.
		*/
		virtual std::shared_ptr<VulkanPipeline> CreatePipeline(
			std::shared_ptr<Material>        material  ,
			VkPipelineLayout&                layout    ,
			std::shared_ptr<RendererSubPass> subPass
		) override;
	};

}