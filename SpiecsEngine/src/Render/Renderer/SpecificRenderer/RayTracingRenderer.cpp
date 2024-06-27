/**
* @file RayTracingRenderer.cpp.
* @brief The RayTracingRenderer Class Implementation.
* @author Spiecs.
*/

#include "Pchheader.h"
#include "RayTracingRenderer.h"
#include "PreRenderer.h"
#include "Render/Vulkan/VulkanRayTracing.h"
#include "Core/Library/MemoryLibrary.h"

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
		SPIECS_PROFILE_ZONE;

		m_VulkanRayTracing = std::make_unique<VulkanRayTracing>(m_VulkanState);

		vkGetRayTracingShaderGroupHandlesKHR  = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(vkGetInstanceProcAddr(vulkanState.m_Instance, "vkGetRayTracingShaderGroupHandlesKHR"));
		vkCmdTraceRaysKHR                     = reinterpret_cast<PFN_vkCmdTraceRaysKHR>(vkGetInstanceProcAddr(vulkanState.m_Instance, "vkCmdTraceRaysKHR"));
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
		.AddStorageTexture(1, 1, VK_SHADER_STAGE_RAYGEN_BIT_KHR, { "Ray" }, VK_FORMAT_R32G32B32A32_SFLOAT)
		.Build(m_VulkanRayTracing->GetAccelerationStructure());
	}

	std::shared_ptr<VulkanPipeline> RayTracingRenderer::CreatePipeline(
		std::shared_ptr<Material>         material ,
		VkPipelineLayout&                 layout   ,
		std::shared_ptr<RendererSubPass>  subPass
	)
	{
		SPIECS_PROFILE_ZONE;

		PipelineConfigInfo pipelineConfig{};

		pipelineConfig.pipelineLayout              = layout;

		pipelineConfig.viewport.x = 0.0f;
		pipelineConfig.viewport.y = static_cast<float>(m_Device->GetSwapChainSupport().surfaceSize.height);
		pipelineConfig.viewport.width = static_cast<float>(m_Device->GetSwapChainSupport().surfaceSize.width);
		pipelineConfig.viewport.height = -static_cast<float>(m_Device->GetSwapChainSupport().surfaceSize.height);
		pipelineConfig.viewport.minDepth = 0.0f;
		pipelineConfig.viewport.maxDepth = 1.0f;

		if (SlateSystem::GetRegister())
		{
			ImVec2 viewPortSize = SlateSystem::GetRegister()->GetViewPort()->GetPanelSize();

			pipelineConfig.viewport.y = viewPortSize.y;
			pipelineConfig.viewport.width = viewPortSize.x;
			pipelineConfig.viewport.height = -viewPortSize.y;
		}
		pipelineConfig.scissor.offset = { 0, 0 };

		pipelineConfig.scissor.extent = m_Device->GetSwapChainSupport().surfaceSize;

		pipelineConfig.viewportInfo.pViewports = &pipelineConfig.viewport;
		pipelineConfig.viewportInfo.pScissors = &pipelineConfig.scissor;

		return std::make_shared<VulkanRayTracingPipeline>(
			m_VulkanState,
			material->GetName(),
			material->GetShaderPath(),
			pipelineConfig
		);
	}

	void RayTracingRenderer::Render(TimeStep& ts, FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

		static bool in = true;

		if (in)
		{
			CreateBottomLevelAS(frameInfo);
			CreateTopLevelAS();
			CreateRTShaderBindingTable();
			in = false;
		}

		RenderBehaverBuilder builder{ this ,frameInfo.m_FrameIndex, frameInfo.m_Imageindex };

		builder.BindDescriptorSet(DescriptorSetManager::GetByName("RayTracing"), "RayTracingRenderer.RayTracing.Default", VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR);

		builder.BindPipeline("RayTracingRenderer.RayTracing.Default", VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR);
		
		uint32_t width = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().x);
		uint32_t height = static_cast<uint32_t>(SlateSystem::GetRegister()->GetViewPort()->GetPanelSize().y);

		/*
		* @attention VulkanSDK Version Below 1.3.283 not allow dynamic state in mixing raytracing pipeline and custom graphic pipeline.
		* @see https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/8038.
		*/
		vkCmdTraceRaysKHR(m_VulkanState.m_CommandBuffer[frameInfo.m_FrameIndex], &m_RgenRegion, &m_MissRegion, &m_HitRegion, &m_CallRegion, width, height, 1); 
	}

	void RayTracingRenderer::CreateBottomLevelAS(FrameInfo& frameInfo)
	{
		SPIECS_PROFILE_ZONE;

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
		SPIECS_PROFILE_ZONE;

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

	void RayTracingRenderer::CreateRTShaderBindingTable()
	{
		SPIECS_PROFILE_ZONE;

		uint32_t missCount { 1 };
		uint32_t hitCount  { 1 };
		auto     handleCount                    = 1 + missCount + hitCount;
		uint32_t handleSize                     = m_Device->GetRTPipelineProperties().shaderGroupHandleSize;

		/**
		* @brief The SBT(buffer) need to have starting groups to be aligned and handles in the group to be aligned.
		*/
		uint32_t handleSizeAligned              = MemoryLibrary::align_up(handleSize, m_Device->GetRTPipelineProperties().shaderGroupHandleAlignment);

		m_RgenRegion.stride                     = MemoryLibrary::align_up(handleSizeAligned, m_Device->GetRTPipelineProperties().shaderGroupBaseAlignment);
		m_RgenRegion.size                       = m_RgenRegion.stride;  // The size member of pRayGenShaderBindingTable must be equal to its stride member

		m_MissRegion.stride                     = handleSizeAligned;
		m_MissRegion.size                       = MemoryLibrary::align_up(missCount * handleSizeAligned, m_Device->GetRTPipelineProperties().shaderGroupBaseAlignment);

		m_HitRegion.stride                      = handleSizeAligned;
		m_HitRegion.size                        = MemoryLibrary::align_up(hitCount * handleSizeAligned, m_Device->GetRTPipelineProperties().shaderGroupBaseAlignment);

		/**
		* @brief Get the shader group handles.
		*/ 
		uint32_t dataSize                       = handleCount * handleSize;
		std::vector<uint8_t> handles(dataSize);
		VK_CHECK(vkGetRayTracingShaderGroupHandlesKHR(m_VulkanState.m_Device, m_Pipelines["RayTracingRenderer.RayTracing.Default"]->GetPipeline(), 0, handleCount, dataSize, handles.data()));

		// Allocate a buffer for storing the SBT.
		VkDeviceSize sbtSize = m_RgenRegion.size + m_MissRegion.size + m_HitRegion.size + m_CallRegion.size;

		m_RTSBTBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState, 
			sbtSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT | 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | 
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		VulkanDebugUtils::SetObjectName(VK_OBJECT_TYPE_BUFFER, m_RTSBTBuffer->Get(), m_VulkanState.m_Device, "SBT Buffer");

		// Find the SBT addresses of each group
		VkBufferDeviceAddressInfo info {};
		info.sType                              = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		info.buffer                             = m_RTSBTBuffer->Get();

		m_RgenRegion.deviceAddress              = m_RTSBTBuffer->GetAddress();
		m_MissRegion.deviceAddress              = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size;
		m_HitRegion.deviceAddress               = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size + m_MissRegion.size;

		// Helper to retrieve the handle data
		auto getHandle = [&](int i) { return handles.data() + i * handleSize; };

		void* data;
		vkMapMemory(m_VulkanState.m_Device, m_RTSBTBuffer->GetMomory(), 0, sbtSize, 0, &data);

		// Map the SBT buffer and write in the handles.
		uint8_t* pSBTBuffer                     = reinterpret_cast<uint8_t*>(data);
		uint8_t* pData { nullptr };
		uint32_t handleIdx { 0 };

		// Raygen
		pData = pSBTBuffer;
		memcpy(pData, getHandle(handleIdx++), handleSize);

		// Miss
		pData = pSBTBuffer + m_RgenRegion.size;
		for (uint32_t c = 0; c < missCount; c++)
		{
			memcpy(pData, getHandle(handleIdx++), handleSize);
			pData += m_MissRegion.stride;
		}

		// Hit
		pData = pSBTBuffer + m_RgenRegion.size + m_MissRegion.size;
		for (uint32_t c = 0; c < hitCount; c++)
		{
			memcpy(pData, getHandle(handleIdx++), handleSize);
			pData += m_HitRegion.stride;
		}

		vkUnmapMemory(m_VulkanState.m_Device, m_RTSBTBuffer->GetMomory());
	}
}