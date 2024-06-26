/**
* @file RayTracingRenderer.cpp.
* @brief The RayTracingRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RayTracingRenderer.h"
#include "PreRenderer.h"
#include "Render/Vulkan/VulkanRayTracing.h"

namespace Spiecs {

	RayTracingRenderer::RayTracingRenderer(
		const std::string&                     rendererName          ,
		VulkanState&                           vulkanState           ,
		std::shared_ptr<VulkanDescriptorPool>  desctiptorPool        ,
		std::shared_ptr<VulkanDevice>          device                ,
		std::shared_ptr<RendererResourcePool>  rendererResourcePool
	)
		: Renderer(rendererName, vulkanState, desctiptorPool, device, rendererResourcePool)
	{
		m_VulkanRayTracing = std::make_unique<VulkanRayTracing>(m_VulkanState);
	}

	void RayTracingRenderer::CreateRendererPass()
	{
		SPIECS_PROFILE_ZONE;

		RendererPassBuilder{ "RayTracing", this }
		.AddSubPass("RayTracing")
		.EndSubPass()
		.Build();
	}

	void RayTracingRenderer::CreateDescriptorSet()
	{
		SPIECS_PROFILE_ZONE;

		DescriptorSetBuilder{ "RayTracing", this }
		.AddAccelerationStructure(1, 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR)
		//.AddStorageTexture<Texture2D>(1, 0, VK_SHADER_STAGE_RAYGEN_BIT_KHR, { "Ray" })
		.Build(m_VulkanRayTracing->GetAccelerationStructure());
	}

	void RayTracingRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		static bool in = true;

		if (in)
		{
			CreateBottomLevelAS(frameInfo);
			CreateTopLevelAS();
			in = false;
		}

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BeginRenderPass();

		builder.EndRenderPass();
	}

	void RayTracingRenderer::CreateBottomLevelAS(FrameInfo& frameInfo)
	{
		/**
		* @brief BLAS - Storing each primitive in a geometry.
		*/
		std::vector<VulkanRayTracing::BlasInput> allBlas;

		/**
		* @brief Iter all MeshComponents.
		*/
		auto& view = frameInfo.m_World->GetRegistry().view<MeshComponent>();
		for (auto& e : view)
		{
			auto& meshComp = frameInfo.m_World->GetRegistry().get<MeshComponent>(e);

			auto blas = meshComp.GetMesh()->CreateMeshPackASInput();
			ContainerLibrary::Append<VulkanRayTracing::BlasInput>(allBlas, blas);
		}

		/**
		* @brief Build BLAS.
		*/
		m_VulkanRayTracing->BuildBLAS(allBlas, VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR);
	}

	void RayTracingRenderer::CreateTopLevelAS()
	{
		std::vector<VkAccelerationStructureInstanceKHR> tlas;
		tlas.reserve(1);

		{
			VkAccelerationStructureInstanceKHR rayInst{};
			rayInst.transform = ToVkTransformMatrixKHR(glm::mat4(1.0f));                 // Position of the instance
			rayInst.instanceCustomIndex = 0;                                                          // gl_InstanceCustomIndexEXT
			rayInst.accelerationStructureReference = m_VulkanRayTracing->GetBlasDeviceAddress(0);
			rayInst.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
			rayInst.mask = 0xFF;                                                       //  Only be hit if rayMask & instance.mask != 0
			rayInst.instanceShaderBindingTableRecordOffset = 0;                                                          // We will use the same hit group for all objects

			tlas.emplace_back(rayInst);
		}

		/**
		* @brief Build TLAS.
		*/
		m_VulkanRayTracing->BuildTLAS(tlas, VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR);
	}
}