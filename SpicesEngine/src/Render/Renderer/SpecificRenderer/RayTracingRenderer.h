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
		static const VkAccelerationStructureKHR& GetAccelerationStructure() { return m_VulkanRayTracing->GetAccelerationStructure(); }

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
		* @param[in] ratTracingInstance VulkanRayTracingInstace.
		*/
		template<typename V>
		void CreateBottomLevelAS(
			FrameInfo& frameInfo, 
			V& view,
			std::shared_ptr<VulkanRayTracing> rayTracingInstance
		);

		/**
		* @brief Create TopLevelAS.
		* @param[in] frameInfo FrameInfo.
		* @param[in] view Component View.
		* @param[in] ratTracingInstance VulkanRayTracingInstance.
		* @param[in] update default false.
		*/
		template<typename V>
		void CreateTopLevelAS(
			FrameInfo& frameInfo, 
			V& view,
			std::shared_ptr<VulkanRayTracing> rayTracingInstance,
			bool update = false
		);

		/**
		* @brief Update TopLevelAS.
		* @param[in] frameInfo FrameInfo.
		* @param[in] ratTracingInstance VulkanRayTracingInstance.
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
		* @param[in] ratTracingInstance VulkanRayTracingInstance.
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
	
	template<typename V>
	inline void RayTracingRenderer::CreateBottomLevelAS(FrameInfo& frameInfo, V& view, std::shared_ptr<VulkanRayTracing> rayTracingInstance)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief BLAS - Storing each primitive in a geometry.
		*/
		std::vector<VulkanRayTracing::BlasInput> allBlas;
		std::shared_ptr<std::unordered_map<std::string, uint32_t>> hitGroups = std::make_shared<std::unordered_map<std::string, uint32_t>>();
		
		/**
		* @brief Iter all MeshComponents.
		*/
		for (auto& e : view)
		{
			if (!frameInfo.m_World->GetRegistry().any_of(e)) continue;

			auto& meshComp = frameInfo.m_World->GetRegistry().get<MeshComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {

				auto blas = v->MeshPackToVkGeometryKHR(allBlas.size());
				allBlas.emplace_back(blas);

				return false;
			});

			meshComp.GetMesh()->AddMaterialToHitGroup(*hitGroups);
		}

		/**
		* @brief Cache this frame hit groups.
		*/
		SetHitGroupsCache(hitGroups);

		rayTracingInstance->SetHitGroups(hitGroups);

		/**
		* @brief Build BLAS.
		*/
		rayTracingInstance->BuildBLAS(
			allBlas, 
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | 
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR 
			//VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR     // Compress cost too mush time in with large count of meshes.
		);
	}
 
	template<typename V>
	inline void RayTracingRenderer::CreateTopLevelAS(FrameInfo& frameInfo, V& view, std::shared_ptr<VulkanRayTracing> rayTracingInstance, bool update)
	{
		SPICES_PROFILE_ZONE;

		std::vector<VkAccelerationStructureInstanceKHR> tlas;

		int index = 0;
		auto& desc = rayTracingInstance->GetMeshDesc().attributes;
		desc->resize(SpicesShader::MESH_BUFFER_MAXNUM, 0);

		for (auto& e : view)
		{
			if (!frameInfo.m_World->GetRegistry().any_of(e)) continue;

			MeshComponent meshComp;
			TransformComponent tranComp;

			std::tie(meshComp, tranComp) = frameInfo.m_World->GetRegistry().get<MeshComponent, TransformComponent>(e);

			meshComp.GetMesh()->GetPacks().for_each([&](const uint32_t& k, const std::shared_ptr<MeshPack>& v) {

				VkAccelerationStructureInstanceKHR                            rayInst{};
				rayInst.transform                                           = ToVkTransformMatrixKHR(tranComp.GetModelMatrix());          // Position of the instance
				rayInst.instanceCustomIndex                                 = index;                                                      // gl_InstanceCustomIndexEXT
				rayInst.accelerationStructureReference                      = rayTracingInstance->GetBlasDeviceAddress(v->GetBlasIndex());
				rayInst.flags                                               = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
				rayInst.mask                                                = 0xFF;                                                       // Only be hit if rayMask & instance.mask != 0
				rayInst.instanceShaderBindingTableRecordOffset              = v->GetHitShaderHandle();                                    // We will use the same hit group for all objects

				tlas.push_back(rayInst);

				(*desc)[index] = v->GetMeshDesc().GetBufferAddress();

				index += 1;
				return false;
			});
		}

		rayTracingInstance->GetMeshDesc().CreateBuffer("MeshDescBuffer", VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

		/**
		* @brief Build TLAS.
		*/
		rayTracingInstance->BuildTLAS(
			tlas,
			VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR      |
			VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR  ,
			update
		);
	}
}