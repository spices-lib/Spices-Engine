#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanCommandBuffer.h"
#include "VulkanBuffer.h"

namespace Spiecs {

	class VulkanRayTracing : public VulkanObject
	{
	public:

		struct BlasInput
		{
			// Data used to build acceleration structure geometry
			std::vector<VkAccelerationStructureGeometryKHR>       asGeometry;
			std::vector<VkAccelerationStructureBuildRangeInfoKHR> asBuildOffsetInfo;
			VkBuildAccelerationStructureFlagsKHR                  flags{ 0 };
		};

		struct AccelKHR
		{
			VkAccelerationStructureKHR                  accel = VK_NULL_HANDLE;
			std::shared_ptr<VulkanBuffer>               buffer;

			void FreeBuffer()
			{
				buffer = nullptr;
			}
		};

		struct BuildAccelerationStructure
		{
			VkAccelerationStructureBuildGeometryInfoKHR buildInfo{ VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR };
			VkAccelerationStructureBuildSizesInfoKHR sizeInfo{ VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR };
			const VkAccelerationStructureBuildRangeInfoKHR* rangeInfo;
			AccelKHR                                  as;  // result acceleration structure
			AccelKHR                                 cleanupAS;
		};
		
	public:

		VulkanRayTracing(VulkanState& vulkanState);

		virtual ~VulkanRayTracing();

		void Destroy();

		VkAccelerationStructureKHR GetAccelerationStructure() const { return m_tlas.accel; };
		VkDeviceAddress GetBlasDeviceAddress(uint32_t blasId);

		/**
		* @brief Create all the BLAS from the vector of BlasInput.
		* There will be one BLAS per input-vector entry.
		* There will be as many BLAS as input.size().
		* The resulting BLAS (along with the inputs used to build) are stored in m_blas,
		* and can be referenced by index.
		* if flag has the 'Compact' flag, the BLAS will be compacted.
		*/
		void BuildBLAS(
			const std::vector<BlasInput>&        input                                                             , 
			VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR
		);

		void UpdateBlas(uint32_t blasIdx, BlasInput& blas, VkBuildAccelerationStructureFlagsKHR flags);

		/**
		* @brief Creating the top-level acceleration structure from the vector of Instance.
		* The resulting TLAS will be stored in m_tlas.
		* update is to rebuild the Tlas with updated matrices.
		*/
		void BuildTLAS(
			const std::vector<VkAccelerationStructureInstanceKHR>& instances,
			VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			bool update = false
		);

#ifdef VK_NV_ray_tracing_motion_blur

		void BuildTLAS(
			const std::vector<VkAccelerationStructureMotionInstanceNV>& instances,
			VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_MOTION_BIT_NV,
			bool update = false
		);

#endif

		/**
		* @brief Build TLAS from an array of VkAccelerationStructureInstanceKHR.
		* Use motion=true with VkAccelerationStructureMotionInstanceNV.
		* The resulting TLAS will be stored in m_tlas.
		* update is to rebuild the Tlas with updated matrices, flag must have the 'allow_update'
		*/
		template<class T>
		void BuildTLAS(
			const std::vector<T>& instances,
			VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			bool update = false,
			bool motion = false
		);

		/**
		* @brief Low level of Tlas creation.
		*  Creating the TLAS, called by buildTlas.
		*/
		void CmdCreateTLAS(
			VkCommandBuffer                      cmdBuf         ,   // Command buffer.
			uint32_t                             countInstance  ,   // number of instances.
			VkDeviceAddress                      instBufferAddr ,   // Buffer address of instances.
			std::unique_ptr<VulkanBuffer>&       scratchBuffer  ,   // ScratchBuffer Buffer.
			VkBuildAccelerationStructureFlagsKHR flags          ,   // Build creation flag.
			bool                                 update         ,   // Update == animation.
			bool                                 motion             // Motion Blur.
		);

	private:

		/**
		* @brief Creating the bottom level acceleration structure for all indices of `buildAs` vector.
		* The array of BuildAccelerationStructure was created in buildBlas and the vector of
		* indices limits the number of BLAS to create at once. This limits the amount of
		* memory needed when compacting the BLAS.
		*/
		void CmdCreateBLAS(VkCommandBuffer           cmdBuf,
			std::vector<uint32_t>                    indices,
			std::vector<BuildAccelerationStructure>& buildAs,
			VkDeviceAddress                          scratchAddress,
			VkQueryPool                              queryPool
		);

		/**
		* @brief Create and replace a new acceleration structure and buffer based on the size retrieved by the Query.
		* We have to wait until all BLAS are built, to make a copy in the more suitable memory space.
		* This is the reason why we used m_cmdPool.submitAndWait(cmdBuf) before calling this function.
		*/
		void CmdCompactBLAS(
			VkCommandBuffer                          cmdBuf,
			std::vector<uint32_t>                    indices,
			std::vector<BuildAccelerationStructure>& buildAs,
			VkQueryPool                              queryPool
		);

		void DestroyNonCompacted(std::vector<uint32_t> indices, std::vector<BuildAccelerationStructure>& buildAs);
		bool hasFlag(VkFlags item, VkFlags flag) { return (item & flag) == flag; }
		AccelKHR CreateAcceleration(VkAccelerationStructureCreateInfoKHR& accel);

	private:

		std::vector<AccelKHR> m_blas;  // Bottom-level acceleration structure
		AccelKHR              m_tlas;  // Top-level acceleration structure


		PFN_vkCreateAccelerationStructureKHR                   vkCreateAccelerationStructureKHR;
		PFN_vkDestroyAccelerationStructureKHR                  vkDestroyAccelerationStructureKHR;
		PFN_vkCmdBuildAccelerationStructuresKHR                vkCmdBuildAccelerationStructuresKHR;
		PFN_vkCopyAccelerationStructureKHR                     vkCopyAccelerationStructureKHR;
		PFN_vkGetAccelerationStructureDeviceAddressKHR         vkGetAccelerationStructureDeviceAddressKHR;
		PFN_vkCmdWriteAccelerationStructuresPropertiesKHR      vkCmdWriteAccelerationStructuresPropertiesKHR;
		PFN_vkGetAccelerationStructureBuildSizesKHR            vkGetAccelerationStructureBuildSizesKHR;
		PFN_vkCmdCopyAccelerationStructureKHR                  vkCmdCopyAccelerationStructureKHR;
	};

	template<class T>
	inline void VulkanRayTracing::BuildTLAS(
		const std::vector<T>&                instances  , 
		VkBuildAccelerationStructureFlagsKHR flags      , 
		bool                                 update     , 
		bool                                 motion
	)
	{
		/**
		* @brief Cannot call buildTlas twice except to update.
		*/
		assert(m_tlas.accel == VK_NULL_HANDLE || update);
		uint32_t countInstance = static_cast<uint32_t>(instances.size());

		/**
		* @brief Create a buffer holding the actual instance data (matrices++) for use by the AS builder.
		*/
		VulkanBuffer instancesBuffer(
			m_VulkanState,
			sizeof(T) * instances.size(),
			VK_BUFFER_USAGE_TRANSFER_DST_BIT |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | 
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR,
			0
		);

		VulkanBuffer stagingBuffer(
			m_VulkanState,
			sizeof(T) * instances.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		void* data;
		vkMapMemory(m_VulkanState.m_Device, stagingBuffer.GetMomory(), 0, sizeof(T) * instances.size(), 0, &data);
		memcpy(data, instances.data(), sizeof(T) * instances.size());
		vkUnmapMemory(m_VulkanState.m_Device, stagingBuffer.GetMomory());

		instancesBuffer.CopyBuffer(stagingBuffer.Get(), instancesBuffer.Get(), sizeof(T) * instances.size());

		VkDeviceAddress instBufferAddr = instancesBuffer.GetAddress();

		/**
		* @brief Command buffer to create the TLAS.
		*/
		std::unique_ptr<VulkanBuffer> scratchBuffer = nullptr;
		VulkanCommandBuffer::CustomCmd(m_VulkanState, [&](VkCommandBuffer& commandBuffer) {

			/**
			* @brief Make sure the copy of the instance buffer are copied before triggering the acceleration structure build.
			*/
			VkMemoryBarrier barrier{};
			barrier.sType                        = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
			barrier.srcAccessMask                = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask                = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;

			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
				0,
				1,
				&barrier,
				0,
				nullptr,
				0,
				nullptr
			);

			CmdCreateTLAS(commandBuffer, countInstance, instBufferAddr, scratchBuffer, flags, update, motion);
		});
	}
}