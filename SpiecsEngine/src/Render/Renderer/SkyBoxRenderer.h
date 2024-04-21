/**
* @file SkyBoxRenderer.h.
* @brief The SkyBoxRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Renderer.h"

namespace Spiecs {

	/**
	* @brief SkyBoxRenderer Class.
	* This class defines the skybox component render behaver.
	*/
	class SkyBoxRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		SkyBoxRenderer(const std::string& rendererName, VulkanState& vulkanState, std::shared_ptr<VulkanDescriptorPool> desctiptorPool, std::shared_ptr<VulkanDevice> device)
			: Renderer(rendererName, vulkanState, desctiptorPool, device) {};

		/**
		* @brief Destructor Function.
		*/
		virtual ~SkyBoxRenderer() {};

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(FrameInfo& frameInfo) override;

	private:

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific renderpass.
		*/
		virtual void CreateRenderPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific pipelinelayout and buffer type descriptor.
		*/
		virtual void CreatePipelineLayoutAndDescriptor() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific pipeline.
		* @param[in] renderPass Renderer specific renderpass.
		*/
		virtual void CreatePipeline(VkRenderPass renderPass) override;

	private:

		/**
		* @brief This struct placed the local buffer data.Specific for SkyBoxRenderer.
		*/
		struct SpecificCollection : public Collection
		{
		private:

			/**
			* @brief VertexSahder Stage uniform buffer.
			*/
			std::unique_ptr<VulkanBuffer> m_VertRendererUBO;

		public:

			/**
			* @brief The interface of how to map the local buffer with specific set and binding.
			* @param[in] set Specific set.
			* @param[in] binding Specific binding.
			* @return Returns the local buffer smart pointor.
			*/
			virtual std::unique_ptr<VulkanBuffer>& GetBuffer(uint32_t set, uint32_t binding) override;
		};
	};

}