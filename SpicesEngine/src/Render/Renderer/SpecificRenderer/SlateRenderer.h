/**
* @file SlateRenderer.h.
* @brief The SlateRenderer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spices {

	/**
	* @brief SlateRenderer Class.
	* This class defines the imgui render behaves.
	*/
	class SlateRenderer : public Renderer
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
		SlateRenderer(
			const std::string&                           rendererName          ,
			VulkanState&                                 vulkanState           ,
			const std::shared_ptr<VulkanDescriptorPool>& descriptorPool        ,
			const std::shared_ptr<VulkanDevice>&         device                ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~SlateRenderer() override { ShutdownImgui(); }

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] ts TimeStep.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

		/**
		* @brief Get slate material pipeline.
		* @param[in] name material name.
		* @return Returns material pipeline.
		*/
		static std::shared_ptr<VulkanPipeline> GetPipeline(const std::string& name);

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

		/**
		* @brief The interface is inherited from Renderer.
		* Create Material Specific Pipeline.
		* @param[in] material Registry material.
		* @param[in] layout VkPipelineLayout.
		* @param[in] subPass Handled subPass.
		*/
		virtual void CreatePipeline(
			std::shared_ptr<Material>        material  ,
			VkPipelineLayout&                layout    ,
			std::shared_ptr<RendererSubPass> subPass
		) override;

		/**
		* @brief Rewrite Renderer OnSystemInitialize to add other code.
		*/
		virtual void OnSystemInitialize() override;

		/**
		* @brief This interface is called on Window resized over (registry by swapchain).
		* If the specific renderer uses swapchain image attachment during CreateRenderPass(),
		* this interface needs to override, callOnSlateResize here just will be fine.
		*/
		virtual void OnWindowResizeOver() override;

	private:

		/**
		* @brief Init Imgui.
		*/
		void InitImgui() const;

		/**
		* @brief Shutdown Imgui.
		*/
		void ShutdownImgui();

		/**
		* @brief Begin Imgui frame.
		*/
		void BeginImguiFrame();

		/**
		* @brief End Imgui frame.
		* @param[in] index CommandBuffer index.
		*/
		void EndImguiFrame(uint32_t index) const;

		/**
		* @brief End Imgui frame.
		* @param[in] index CommandBuffer index.
		*/
		void EndImguiFrameAsync(uint32_t index);
	};
}