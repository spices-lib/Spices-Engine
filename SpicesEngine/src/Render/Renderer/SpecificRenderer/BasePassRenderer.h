/**
* @file BasePassRenderer.h.
* @brief The BasePassRenderer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spices {

	namespace BasePassR {

		struct MeshDescBuffer
		{
			std::vector<SpicesShader::MeshDesc> descs;
		};
	}

	/**
	* @brief BasePassRenderer Class.
	* This class defines the base pass render behaves.
	*/
	class BasePassRenderer : public Renderer
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
		BasePassRenderer(
			const std::string&                           rendererName        ,
			VulkanState&                                 vulkanState         ,
			const std::shared_ptr<VulkanDescriptorPool>& descriptorPool      ,
			const std::shared_ptr<VulkanDevice>&         device              ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool
		)
			: Renderer(rendererName, vulkanState, descriptorPool, device, rendererResourcePool, true)
			, m_View(nullptr)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~BasePassRenderer() override = default;

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] ts TimeStep.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

	private:
		 
		/**
		* @brief The interface is inherited from Renderer.
		* Create Specific Renderer pass.
		*/
		virtual void CreateRendererPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific descriptor set for sub pass.
		*/
		virtual void CreateDescriptorSet() override;

		/**
		* @brief This interface is called during OnSystemInitialize().
		* Create Device Generated Commands Layout.
		*/
		virtual void CreateDeviceGeneratedCommandsLayout() override;

		/**
		* @breif This interface is called on world mark query tick (registry by MeshComponent).
		*/
		virtual void OnMeshAddedWorld() override;

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
		* @brief Create device generated command Pipeline.
		* @param[in] pipelineName Pipeline's name.
		* @param[in] materialName Material's name.
		* @param[in] layout PipelineLayout.
		* @param[in] subPass RendererSubPass.
		* @param[in] indirectPtr VulkanDeviceGeneratedCommandsNV.
		*/
		virtual void CreateDeviceGeneratedCommandsPipeline(
			const std::string&               pipelineName ,
			const std::string&               materialName ,
			VkPipelineLayout&                layout       ,
			std::shared_ptr<RendererSubPass> subPass      ,
			VulkanDeviceGeneratedCommandsNV* indirectPtr
		) override;

	private:

		/**
		* @brief World entities view.
		*/
		std::shared_ptr<std::vector<uint32_t>> m_View;
	};

}