/**
* @file VulkanRayTracing.cpp.
* @brief The VulkanRayTracing Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "VulkanRayTracing.h"
#include "VulkanMemoryAllocator.h"
#include "VulkanQueryPool.h"
#include "VulkanDevice.h"
#include "Core/Library/MemoryLibrary.h"

namespace Spices {

	VulkanRayTracing::VulkanRayTracing(VulkanState& vulkanState)
		: VulkanObject(vulkanState)
	{}

	const VkAccelerationStructureKHR VulkanRayTracing::GetAccelerationStructure() const
	{
		SPICES_PROFILE_ZONE;

		if (m_tlas.accel)
		{
			return m_tlas.accel->Get();
		}
		
		return nullptr;
	}

	void VulkanRayTracing::BuildBLAS(
		const std::vector<BlasInput>&        input , 
		VkBuildAccelerationStructureFlagsKHR flags
	)
	{
		SPICES_PROFILE_ZONE;

		const auto       nbBlas         = static_cast<uint32_t>(input.size());
		VkDeviceSize     asTotalSize    = 0;     
		uint32_t         nbCompactions  = 0;   
		VkDeviceSize     maxScratchSize = 0;  

		/**
		* @brief Preparing the information for the acceleration build commands..
		*/
		std::vector<BuildAccelerationStructure> buildAs(nbBlas);
		for (uint32_t idx = 0; idx < nbBlas; idx++)
		{
			/* 
			* @brief Filling partially the VkAccelerationStructureBuildGeometryInfoKHR for querying the build sizes.
			* Other information will be filled in the createBlas (see #2)
			*/ 
			buildAs[idx].buildInfo.type              = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
			buildAs[idx].buildInfo.mode              = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
			buildAs[idx].buildInfo.flags             = input[idx].flags | flags;
			buildAs[idx].buildInfo.geometryCount     = static_cast<uint32_t>(input[idx].asGeometry.size());
			buildAs[idx].buildInfo.pGeometries       = input[idx].asGeometry.data();
			buildAs[idx].rangeInfo                   = input[idx].asBuildOffsetInfo.data();  /*@brief Build range information.*/
			buildAs[idx].as                          = input[idx].accel;

			/*
			* @brief Finding sizes to create acceleration structures and scratch.
			*/ 
			std::vector<uint32_t> maxPrimCount(input[idx].asBuildOffsetInfo.size());
			for (auto tt = 0; tt < input[idx].asBuildOffsetInfo.size(); tt++)
			{
				maxPrimCount[tt] = input[idx].asBuildOffsetInfo[tt].primitiveCount;  /*@brief Number of primitives / triangles*/
			}

			/**
			* @brief Get ASBuildSize.
			*/
			m_VulkanState.m_VkFunc.vkGetAccelerationStructureBuildSizesKHR(
				m_VulkanState.m_Device, 
				VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
				&buildAs[idx].buildInfo, 
				maxPrimCount.data(), 
				&buildAs[idx].sizeInfo
			);

			/**
			* @brief Extra info.
			*/ 
			asTotalSize      += buildAs[idx].sizeInfo.accelerationStructureSize;
			maxScratchSize    = std::max(maxScratchSize, buildAs[idx].sizeInfo.buildScratchSize);
			nbCompactions    += hasFlag(buildAs[idx].buildInfo.flags, VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_KHR);
		}

		/**
		* @brief Allocate the scratch buffers holding the temporary data of the acceleration structure builder.
		*/ 
		VulkanBuffer scratchBuffer(
			m_VulkanState, 
			"ScratchBuffer",
			maxScratchSize, 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | 
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT        , 
			VMA_MEMORY_PROPERTY_DEDICATED_MEMORY_BIT
		);

		const VkDeviceAddress scratchAddress = scratchBuffer.GetAddress();

		/**
		* @brief Allocate a query pool for storing the needed size for every BLAS compaction.
		*/ 
		std::shared_ptr<VulkanQueryPool> queryPool = nullptr;
		if (nbCompactions > 0)                // Is compaction requested?
		{
			assert(nbCompactions == nbBlas);  // Don't allow mix of on/off compaction

			/**
			* @brief Create Query Pool.
			*/
			queryPool = std::make_shared<VulkanQueryPool>(m_VulkanState, VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, nbBlas);
		}

		/**
		* @brief Batching creation / compaction of BLAS to allow staying in restricted amount of memory.
		*/ 
		std::vector<uint32_t>     indices;                    // Indices of the BLAS to create
		VkDeviceSize              batchSize = 0;
		constexpr VkDeviceSize    batchLimit = 256'000'000;  // 256 MB

		for (uint32_t idx = 0; idx < nbBlas; idx++)
		{
			indices.push_back(idx);
			batchSize += buildAs[idx].sizeInfo.accelerationStructureSize;

			/**
			* @brief Over the limit or last BLAS element.
			*/ 
			if (batchSize >= batchLimit || idx == nbBlas - 1)
			{
				VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](const VkCommandBuffer& commandBuffer) {
					CmdCreateBLAS(commandBuffer, indices, buildAs, scratchAddress, queryPool);				
				});

				if (queryPool)
				{
					VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](const VkCommandBuffer& commandBuffer) {
						CmdCompactBLAS(commandBuffer, indices, buildAs, queryPool);
					});
				}
				
				/**
				* @brief Reset.
				*/ 
				batchSize = 0;
				indices.clear();
			}
		}
	}

	void VulkanRayTracing::UpdateBlas(uint32_t blasIdx, const BlasInput& blas, VkBuildAccelerationStructureFlagsKHR flags) const
	{
		SPICES_PROFILE_ZONE;

		// Preparing all build information, acceleration is filled later
		VkAccelerationStructureBuildGeometryInfoKHR buildInfos{};
		buildInfos.sType                           = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfos.flags                           = flags;
		buildInfos.geometryCount                   = static_cast<uint32_t>(blas.asGeometry.size());
		buildInfos.pGeometries                     = blas.asGeometry.data();
		buildInfos.mode                            = VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;  // UPDATE
		buildInfos.type                            = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		//buildInfos.srcAccelerationStructure        = m_blas[blasIdx].accel;  // UPDATE
		//buildInfos.dstAccelerationStructure        = m_blas[blasIdx].accel;

		// Find size to build on the device
		std::vector<uint32_t> maxPrimCount(blas.asBuildOffsetInfo.size());

		for (auto tt = 0; tt < blas.asBuildOffsetInfo.size(); tt++)
		{
			maxPrimCount[tt] = blas.asBuildOffsetInfo[tt].primitiveCount;  // Number of primitives/triangles
		}

		VkAccelerationStructureBuildSizesInfoKHR sizeInfo{};
		sizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;

		m_VulkanState.m_VkFunc.vkGetAccelerationStructureBuildSizesKHR(
			m_VulkanState.m_Device, 
			VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, 
			&buildInfos,
			maxPrimCount.data(), 
			&sizeInfo
		);

		// Allocate the scratch buffer and setting the scratch info
		VulkanBuffer scratchBuffer(
			m_VulkanState, 
			"ScratchBuffer",
			sizeInfo.buildScratchSize, 
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, 
			0
		);

		VkBufferDeviceAddressInfo bufferInfo{};
		bufferInfo.sType                               = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		bufferInfo.buffer                              = scratchBuffer.Get();
		buildInfos.scratchData.deviceAddress           = vkGetBufferDeviceAddress(m_VulkanState.m_Device, &bufferInfo);

		std::vector<const VkAccelerationStructureBuildRangeInfoKHR*> pBuildOffset(blas.asBuildOffsetInfo.size());
		for (size_t i = 0; i < blas.asBuildOffsetInfo.size(); i++)
		{
			pBuildOffset[i] = &blas.asBuildOffsetInfo[i];
		}

		// Update the instance buffer on the device side and build the TLAS
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](const VkCommandBuffer& commandBuffer) {
			// Update the acceleration structure. Note the VK_TRUE parameter to trigger the update,
			// and the existing BLAS being passed and updated in place
			m_VulkanState.m_VkFunc.vkCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildInfos, pBuildOffset.data());
		});
	}

	void VulkanRayTracing::BuildTLAS(
		const std::vector<VkAccelerationStructureInstanceKHR>&  instances , 
		VkBuildAccelerationStructureFlagsKHR                    flags     , 
		bool                                                    update
	)
	{
		SPICES_PROFILE_ZONE;

		BuildTLAS(instances, flags, update, false);
	}

	void VulkanRayTracing::CmdCreateTLAS(
		VkCommandBuffer                       cmdBuf           , 
		uint32_t                              countInstance    , 
		VkDeviceAddress                       instBufferAddr   , 
		std::unique_ptr<VulkanBuffer>&        scratchBuffer    ,
		VkBuildAccelerationStructureFlagsKHR  flags            , 
		bool                                  update           , 
		bool                                  motion		  
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Wraps a device pointer to the above uploaded instances.
		*/
		VkAccelerationStructureGeometryInstancesDataKHR instancesVk{};
		instancesVk.sType                          = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
		instancesVk.data.deviceAddress             = instBufferAddr;

		/**
		* @brief Put the above into a VkAccelerationStructureGeometryKHR.We need to put the instances struct in a union and label it as instance data.
		*/
		VkAccelerationStructureGeometryKHR topASGeometry{};
		topASGeometry.sType                        = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		topASGeometry.geometryType                 = VK_GEOMETRY_TYPE_INSTANCES_KHR;
		topASGeometry.geometry.instances           = instancesVk;

		/**
		* @brief Find sizes.
		*/
		VkAccelerationStructureBuildGeometryInfoKHR buildInfo{};
		buildInfo.sType                            = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfo.flags                            = flags;
		buildInfo.geometryCount                    = 1;
		buildInfo.pGeometries                      = &topASGeometry;
		buildInfo.mode                             = update ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
		buildInfo.type                             = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
		buildInfo.srcAccelerationStructure         = VK_NULL_HANDLE;

		VkAccelerationStructureBuildSizesInfoKHR sizeInfo{};
		sizeInfo.sType                             = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;

		m_VulkanState.m_VkFunc.vkGetAccelerationStructureBuildSizesKHR(
			m_VulkanState.m_Device, 
			VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, 
			&buildInfo,
			&countInstance, 
			&sizeInfo
		);

#ifdef VK_NV_ray_tracing_motion_blur

		VkAccelerationStructureMotionInfoNV motionInfo{};
		motionInfo.sType                           = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MOTION_INFO_NV;
		motionInfo.maxInstances                    = countInstance;

#endif

		/**
		* @brief Create TLAS.
		*/ 
		if (update == false)
		{
			VkAccelerationStructureCreateInfoKHR createInfo{};
			createInfo.sType                       = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
			createInfo.type                        = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
			createInfo.size                        = sizeInfo.accelerationStructureSize;

#ifdef VK_NV_ray_tracing_motion_blur

			if (motion)
			{
				createInfo.createFlags             = VK_ACCELERATION_STRUCTURE_CREATE_MOTION_BIT_NV;
				createInfo.pNext                   = &motionInfo;
			}

#endif

			m_tlas = CreateAcceleration(createInfo);
		}

		/**
		* @brief Allocate the scratch memory.
		*/ 
		scratchBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState,
			"ScratchBuffer",
			sizeInfo.buildScratchSize,
			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VMA_MEMORY_PROPERTY_DEDICATED_MEMORY_BIT
		);

		VkDeviceAddress scratchAddress = scratchBuffer->GetAddress();

		/**
		* @brief Update build information.
		*/ 
		buildInfo.srcAccelerationStructure         = update ? m_tlas.accel->Get() : VK_NULL_HANDLE;
		buildInfo.dstAccelerationStructure         = m_tlas.accel->Get();
		buildInfo.scratchData.deviceAddress        = scratchAddress;

		/**
		* @brief Build Offsets info : n instances.
		*/ 
		VkAccelerationStructureBuildRangeInfoKHR buildOffsetInfo{};
		buildOffsetInfo.primitiveCount             = countInstance;
		buildOffsetInfo.primitiveOffset            = 0; 
		buildOffsetInfo.firstVertex                = 0;
		buildOffsetInfo.transformOffset            = 0;

		const VkAccelerationStructureBuildRangeInfoKHR* pBuildOffsetInfo = &buildOffsetInfo;

		/**
		* @brief Build the TLAS.
		*/
		m_VulkanState.m_VkFunc.vkCmdBuildAccelerationStructuresKHR(cmdBuf, 1, &buildInfo, &pBuildOffsetInfo);
	}

	std::shared_ptr<std::unordered_map<std::string, uint32_t>> VulkanRayTracing::GetHitGroups()
	{
		SPICES_PROFILE_ZONE;

		if (!m_HitGroups)
		{
			m_HitGroups = std::make_shared<std::unordered_map<std::string, uint32_t>>();
		}

		return m_HitGroups;
	}

	void VulkanRayTracing::CreateRTShaderBindingTable(uint32_t rgenCount, uint32_t missCount, VkPipeline pipeline)
	{
		SPICES_PROFILE_ZONE;

		const uint32_t hitCount                 = static_cast<uint32_t>(m_HitGroups->size());
								                
		const auto handleCount                  = rgenCount + missCount + hitCount;
		const uint32_t handleSize               = VulkanDevice::GetRTPipelineProperties().shaderGroupHandleSize;

		/**
		* @brief The SBT(buffer) need to have starting groups to be aligned and handles in the group to be aligned.
		*/
		const uint32_t handleSizeAligned        = MemoryLibrary::align_up(handleSize, VulkanDevice::GetRTPipelineProperties().shaderGroupHandleAlignment);

		m_RgenRegion.stride                     = MemoryLibrary::align_up(handleSizeAligned, VulkanDevice::GetRTPipelineProperties().shaderGroupBaseAlignment);
		m_RgenRegion.size                       = m_RgenRegion.stride;  // The size member of pRayGenShaderBindingTable must be equal to its stride member

		m_MissRegion.stride                     = handleSizeAligned;
		m_MissRegion.size                       = MemoryLibrary::align_up(missCount * handleSizeAligned, VulkanDevice::GetRTPipelineProperties().shaderGroupBaseAlignment);

		m_HitRegion.stride                      = handleSizeAligned;
		m_HitRegion.size                        = MemoryLibrary::align_up(hitCount * handleSizeAligned, VulkanDevice::GetRTPipelineProperties().shaderGroupBaseAlignment);

		/**
		* @brief Get the shader group handles.
		*/
		const uint32_t dataSize                 = handleCount * handleSize;
		std::vector<uint8_t> handles(dataSize);
		VK_CHECK(m_VulkanState
			.m_VkFunc
			.vkGetRayTracingShaderGroupHandlesKHR(
			m_VulkanState.m_Device , 
			pipeline               , 
			0                      , 
			handleCount            , 
			dataSize               , 
			handles.data()
		))

		/**
		* @brief Allocate a buffer for storing the SBT.
		*/
		VkDeviceSize sbtSize = m_RgenRegion.size + m_MissRegion.size + m_HitRegion.size + m_CallRegion.size;

		m_RTSBTBuffer = std::make_unique<VulkanBuffer>(
			m_VulkanState                                , 
			"SBTBuffer"                                  ,
			sbtSize                                      , 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT             | 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT    | 
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR , 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT          | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		DEBUGUTILS_SETOBJECTNAME(VK_OBJECT_TYPE_BUFFER, (uint64_t)m_RTSBTBuffer->Get(), m_VulkanState.m_Device, "SBT Buffer")

		m_RgenRegion.deviceAddress              = m_RTSBTBuffer->GetAddress();
		m_MissRegion.deviceAddress              = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size;
		m_HitRegion.deviceAddress               = m_RTSBTBuffer->GetAddress() + m_RgenRegion.size + m_MissRegion.size;

		/**
		* @brief Helper to retrieve the handle data.
		*/
		auto getHandle = [&](int i) { return handles.data() + i * handleSize; };

		/**
		* @brief Map the SBT buffer and write in the handles.
		*/
		uint64_t offset                         = 0;
		uint32_t handleIdx                      = 0 ;

		/**
		* @brief Ray Generation.
		*/ 
		for (uint32_t c = 0; c < rgenCount; c++)
		{
			m_RTSBTBuffer->WriteToBuffer(getHandle(handleIdx++), handleSize, offset);
			m_RgenRegion.stride;
		}

		/**
		* @brief Miss.
		*/ 
		offset = m_RgenRegion.size;
		for (uint32_t c = 0; c < missCount; c++)
		{
			m_RTSBTBuffer->WriteToBuffer(getHandle(handleIdx++), handleSize, offset);
			offset += m_MissRegion.stride;
		}

		/**
		* @brief Closest Hit.
		*/ 
		offset = m_RgenRegion.size + m_MissRegion.size;
		for (uint32_t c = 0; c < hitCount; c++)
		{
			m_RTSBTBuffer->WriteToBuffer(getHandle(handleIdx++), handleSize, offset);
			offset += m_HitRegion.stride;
		}
	}

	Attribute<uint64_t>& VulkanRayTracing::GetMeshDesc()
	{
		SPICES_PROFILE_ZONE;

		return m_MeshDesc;
	}

	void VulkanRayTracing::CmdCreateBLAS(
		VkCommandBuffer                            cmdBuf          ,
		const std::vector<uint32_t>&               indices         , 
		std::vector<BuildAccelerationStructure>&   buildAs         , 
		VkDeviceAddress                            scratchAddress  , 
		std::shared_ptr<VulkanQueryPool>           queryPool
	) const
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Querying the compaction size.
		*/
		if (queryPool)  
		{
			queryPool->Reset(cmdBuf);
		}

		uint32_t queryCnt{ 0 };

		/**
		* @brief  creating all the BLAS defined by the index chunk.
		*/
		for (const auto& idx : indices)
		{
			/**
			* @brief Actual allocation of buffer and acceleration structure.
			*/
			VkAccelerationStructureCreateInfoKHR      createInfo{};
			createInfo.sType                        = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
			createInfo.type                         = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
			createInfo.size                         = buildAs[idx].sizeInfo.accelerationStructureSize;  // Will be used to allocate memory.

			*buildAs[idx].as = CreateAcceleration(createInfo);

			/**
			* @brief BuildInfo #2 part.
			*/ 
			buildAs[idx].buildInfo.dstAccelerationStructure  = buildAs[idx].as->accel->Get();          // Setting where the build lands
			buildAs[idx].buildInfo.scratchData.deviceAddress = scratchAddress;                         // All build are using the same scratch buffer

			/**
			* @brief Building the bottom - level - acceleration - structure.
			*/ 
			m_VulkanState.m_VkFunc.vkCmdBuildAccelerationStructuresKHR(cmdBuf, 1, &buildAs[idx].buildInfo, &buildAs[idx].rangeInfo);

			/**
			* @brief Since the scratch buffer is reused across builds, we need a barrier to ensure one build
			* is finished before starting the next one.
			*/
			VkMemoryBarrier barrier{};
			barrier.sType                                  = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
			barrier.srcAccessMask                          = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
			barrier.dstAccessMask                          = VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;

			vkCmdPipelineBarrier(
				cmdBuf, 
				VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
				VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR, 
				0, 
				1, 
				&barrier, 
				0, 
				nullptr, 
				0, 
				nullptr
			);

			/**
			* @brief Add the size query only if needed.
			*/
			if (queryPool)
			{
				/**
				* @brief Add a query to find the 'real' amount of memory needed, use for compaction.
				*/ 
				m_VulkanState.m_VkFunc.vkCmdWriteAccelerationStructuresPropertiesKHR(
					cmdBuf, 
					1,
					&buildAs[idx].buildInfo.dstAccelerationStructure,
					VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_KHR, 
					queryPool->Get(),
					queryCnt++
				);
			}
		}
	}

	void VulkanRayTracing::CmdCompactBLAS(
		VkCommandBuffer                          cmdBuf    ,
		const std::vector<uint32_t>&             indices   , 
		std::vector<BuildAccelerationStructure>& buildAs   , 
		std::shared_ptr<VulkanQueryPool>         queryPool
	) 
		const
	{
		SPICES_PROFILE_ZONE;

		uint32_t queryCtn = 0;

		/**
		* @brief Get the compacted size result back.
		*/
		std::vector<VkDeviceSize> compactSizes(static_cast<uint32_t>(indices.size()));

		/**
		* @brief Get Query Pool results.
		*/
		queryPool->QueryResults(compactSizes.data());

		for (const auto idx : indices)
		{
			buildAs[idx].cleanupAS                          = *buildAs[idx].as;             // previous AS to destroy
			buildAs[idx].sizeInfo.accelerationStructureSize = compactSizes[queryCtn++];     // new reduced size

			/**
			* @brief Creating a compact version of the AS.
			*/ 
			VkAccelerationStructureCreateInfoKHR              asCreateInfo{};
			asCreateInfo.sType                              = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
			asCreateInfo.size                               = buildAs[idx].sizeInfo.accelerationStructureSize;
			asCreateInfo.type                               = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
			*buildAs[idx].as                                = CreateAcceleration(asCreateInfo);

			/**
			* @brief Copy the original BLAS to a compact version.
			*/ 
			VkCopyAccelerationStructureInfoKHR                copyInfo{};
			copyInfo.sType                                  = VK_STRUCTURE_TYPE_COPY_ACCELERATION_STRUCTURE_INFO_KHR;
			copyInfo.src                                    = buildAs[idx].buildInfo.dstAccelerationStructure;
			copyInfo.dst                                    = buildAs[idx].as->accel->Get();
			copyInfo.mode                                   = VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_KHR;

			m_VulkanState.m_VkFunc.vkCmdCopyAccelerationStructureKHR(cmdBuf, &copyInfo);
		}
	}

	AccelKHR VulkanRayTracing::CreateAcceleration(VkAccelerationStructureCreateInfoKHR& accel) const
	{
		SPICES_PROFILE_ZONE;

		AccelKHR resultAccel;

		/**
		* @brief Allocating the buffer to hold the acceleration structure.
		*/
		resultAccel.buffer = std::make_shared<VulkanBuffer>(
			m_VulkanState, 
			"AccelBuffer",
			accel.size, 
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | 
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, 
			0
		);

		/**
		* @brief Setting the buffer.
		*/ 
		accel.buffer = resultAccel.buffer->Get();

		/**
		* @brief Create the acceleration structure.
		*/
		resultAccel.accel = std::make_shared<VulkanAccelerationStructure>(m_VulkanState, accel);

		return resultAccel;
	}

}