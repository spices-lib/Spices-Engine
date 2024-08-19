/**
* @file RayTracingRenderer.h.
* @brief The RayTracingRenderer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Renderer/Renderer.h"

namespace Spices {

	class VulkanRayTracing;

	namespace RayTracingR {
		
		struct MeshDescBuffer
		{
			std::array<uint64_t, MESH_BUFFER_MAXNUM> descs;
		};
		
		struct DirectionalLightBuffer
		{
			std::array<SpicesShader::DirectionalLight, DIRECTIONALLIGHT_BUFFER_MAXNUM> lights;
		};

		struct PointLightBuffer
		{
			std::array<SpicesShader::PointLight, POINTLIGHT_BUFFER_MAXNUM> lights;
		};

		struct PushConstant
		{
			int entityID;
		};
		
	}

	/**
	* @brief RayTracingRenderer Class.
	* This class defines the ray tracing behaves.
	*/
	class RayTracingRenderer : public Renderer
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
		* @param[in] cmdThreadPool ThreadPool of submit Cmd parallel.
		*/
		RayTracingRenderer(
			const std::string&                           rendererName          ,
			VulkanState&                                 vulkanState           ,
			const std::shared_ptr<VulkanDescriptorPool>& descriptorPool        ,
			const std::shared_ptr<VulkanDevice>&         device                ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool  ,
			const std::shared_ptr<VulkanCmdThreadPool>&  cmdThreadPool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~RayTracingRenderer() override = default;

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

		/**
		* @breif This interface is called on worldmarkqueryer tick (registry by MeshComponent).
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
		* @param[in] update , default false.
		*/
		void CreateTopLevelAS(FrameInfo& frameInfo, bool update = false);

		/**
		* @brief Update TopLevelAS.
		* @param[in] frameInfo FrameInfo.
		* @param[in] update , default true.
		* @todo Event Queue.
		*/
		void UpdateTopLevelAS(FrameInfo& frameInfo, bool update = true);

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

		std::unique_ptr<RayTracingR::MeshDescBuffer> m_DescArray;
	};
}