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
		
		struct DirectionalLightBuffer
		{
			std::array<SpicesShader::DirectionalLight, SpicesShader::DIRECTIONALLIGHT_BUFFER_MAXNUM> lights;
		};

		struct PointLightBuffer
		{
			std::array<SpicesShader::PointLight, SpicesShader::POINTLIGHT_BUFFER_MAXNUM> lights;
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
		*/
		RayTracingRenderer(
			const std::string&                           rendererName          ,
			VulkanState&                                 vulkanState           ,
			const std::shared_ptr<VulkanDescriptorPool>& descriptorPool        ,
			const std::shared_ptr<VulkanDevice>&         device                ,
			const std::shared_ptr<RendererResourcePool>& rendererResourcePool
		);

		/**
		* @brief Destructor Function.
		*/
		virtual ~RayTracingRenderer() override;

		/**
		* @brief The interface is inherited from Renderer.
		* @param[in] ts TimeStep.
		* @param[in] frameInfo The current frame data.
		*/
		virtual void Render(TimeStep& ts, FrameInfo& frameInfo) override;

		/**
		* @brief Get RayTracing AccelerationStructure.
		* @return Returns RayTracing AccelerationStructure.
		*/
		static VkAccelerationStructureKHR GetAccelerationStructure();

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

	private:

		/**
		* @brief Create BottomLevelAS with all MeshComponents.
		* @param[in] frameInfo FrameInfo.
		* @param[in] view Component View.
		* @param[in] rayTracingInstance VulkanRayTracingInstance.
		*/
		void CreateBottomLevelAS(
			FrameInfo& frameInfo, 
			std::shared_ptr<std::vector<uint32_t>> view,
			std::shared_ptr<VulkanRayTracing> rayTracingInstance
		);

		/**
		* @brief Create TopLevelAS.
		* @param[in] frameInfo FrameInfo.
		* @param[in] view Component View.
		* @param[in] rayTracingInstance VulkanRayTracingInstance.
		* @param[in] update default false.
		*/
		void CreateTopLevelAS(
			FrameInfo& frameInfo, 
			std::shared_ptr<std::vector<uint32_t>> view,
			std::shared_ptr<VulkanRayTracing> rayTracingInstance,
			bool update = false
		);

		/**
		* @brief Update TopLevelAS.
		* @param[in] frameInfo FrameInfo.
		* @param[in] rayTracingInstance VulkanRayTracingInstance.
		* @param[in] update default true.
		* @todo Event Queue.
		*/
		void UpdateTopLevelAS(
			FrameInfo& frameInfo, 
			std::shared_ptr<VulkanRayTracing> rayTracingInstance,
			bool update = true
		);

		/**
		* @brief Create Shader Binding Table.
		* @param[in] rayTracingInstance VulkanRayTracingInstance.
		*/
		void CreateRTShaderBindingTable(std::shared_ptr<VulkanRayTracing> rayTracingInstance);

	private:

		/**
		* @brief Get HitGroupsCache.
		* @return Returns HitGroupsCache.
		*/
		std::shared_ptr<std::unordered_map<std::string, uint32_t>> GetHitGroupsCache();

		/**
		* @brief Set HitGroupsCache.
		* @param[in] cache HitGroupsCache.
		*/
		void SetHitGroupsCache(std::shared_ptr<std::unordered_map<std::string, uint32_t>> cache) { m_HitGroupsCache = cache; }

	private:

		/**
		* @brief In Used VulkanRayTracing.
		*/
		static std::shared_ptr<VulkanRayTracing> m_VulkanRayTracing;

		/**
		* @brief Hit groups cached data.
		*/
		std::shared_ptr<std::unordered_map<std::string, uint32_t>> m_HitGroupsCache;
	};
}