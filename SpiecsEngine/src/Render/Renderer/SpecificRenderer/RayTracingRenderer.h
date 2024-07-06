/**
* @file RayTracingRenderer.h.
* @brief The RayTracingRenderer Class Definitions.
* @author Spiecs.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spiecs {

	class VulkanRayTracing;

	namespace RayTracingR {
		
		struct MeshDescBuffer
		{
			std::array<SpiecsShader::MeshDesc, MESHBUFFERMAXNUM> descs;
		};

		struct DirectionalLightBuffer
		{
			std::array<SpiecsShader::DirectionalLight, DIRECTIONALLIGHTBUFFERMAXNUM> lights;
		};

		struct PointLightBuffer
		{
			std::array<SpiecsShader::PointLight, POINTLIGHTBUFFERMAXNUM> lights;
		};
	}

	/**
	* @brief RayTracingRenderer Class.
	* This class defines the ray tracing behaver.
	*/
	class RayTracingRenderer : public Renderer
	{
	public:

		/**
		* @brief Constructor Function.
		* Init member veriables.
		* @param[in] rendererName The name of this Renderer.
		* @param[in] vulkanState The core vulkan objects that in use.
		* @param[in] desctiptorPool The DesctiptorPool.
		*/
		RayTracingRenderer(
			const std::string&                     rendererName          ,
			VulkanState&                           vulkanState           ,
			std::shared_ptr<VulkanDescriptorPool>  desctiptorPool        ,
			std::shared_ptr<VulkanDevice>          device                ,
			std::shared_ptr<RendererResourcePool>  rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~RayTracingRenderer() {};

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
		virtual void CreateRendererPass() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create specific descriptorset for subpass.
		*/
		virtual void CreateDescriptorSet() override;

		/**
		* @breif This interface is called on worldmarkqueryer tick (regist by MeshComponent).
		*/
		virtual void OnMeshAddedWorld() override;

		/**
		* @brief The interface is inherited from Renderer.
		* Create Material Specific Pipeline.
		* @todo Complete it by renderer.
		*/
		virtual std::shared_ptr<VulkanPipeline> CreatePipeline(
			std::shared_ptr<Material>        material ,
			VkPipelineLayout&                layout   ,
			std::shared_ptr<RendererSubPass> subPass
		) override;

	private:

		/**
		* @brief Create BottomLevelAS with all MeshComponents.
		* @param[in] frameInfo FrameInfo.
		*/
		void CreateBottomLevelAS(FrameInfo& frameInfo);

		/**
		* @brief Create TopLevelAS.
		* @param[in] frameInfo FrameInfo.
		*/
		void CreateTopLevelAS(FrameInfo& frameInfo);

		/**
		* @brief Create Shader Binding Table.
		* @param[in] frameInfo FrameInfo.
		*/
		void CreateRTShaderBindingTable(FrameInfo& frameInfo);

	private:

		/**
		* @brief VulkanRayTracing.
		*/
		std::unique_ptr<VulkanRayTracing> m_VulkanRayTracing;

		std::unique_ptr<VulkanBuffer> m_RTSBTBuffer;
		std::unordered_map<std::string, uint32_t> m_HitGroups;

		VkStridedDeviceAddressRegionKHR m_RgenRegion{};
		VkStridedDeviceAddressRegionKHR m_MissRegion{};
		VkStridedDeviceAddressRegionKHR m_HitRegion{};
		VkStridedDeviceAddressRegionKHR m_CallRegion{};

		PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
		PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;

		std::unique_ptr<RayTracingR::MeshDescBuffer> m_DescArray;
	};
}