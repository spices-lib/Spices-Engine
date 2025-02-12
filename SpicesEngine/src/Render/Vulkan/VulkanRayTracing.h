/**
* @file VulkanRayTracing.h.
* @brief The VulkanRayTracing Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"
#include "VulkanCommandBuffer.h"
#include "VulkanBuffer.h"
#include "Resources/Mesh/Attribute.h"
#include "../../../assets/Shaders/src/Header/ShaderCommon.h"

namespace Spices {

	/**
	* @brief Forward Declare.
	*/
	class VulkanQueryPool;

	struct MeshDescBuffer
	{
		std::array<uint64_t, SpicesShader::MESH_BUFFER_MAXNUM> descs;
	};

	/**
	* @brief Wrapper of Scene RayTracing (KHR/VK) Features and Data.
	*/
	class VulkanRayTracing : public VulkanObject
	{
	public:

		/**
		* @brief Blas Input data.
		*/
		struct BlasInput
		{
			// Data used to build acceleration structure geometry
			std::vector<VkAccelerationStructureGeometryKHR>       asGeometry;
			std::vector<VkAccelerationStructureBuildRangeInfoKHR> asBuildOffsetInfo;
			VkBuildAccelerationStructureFlagsKHR                  flags{ 0 };
			AccelKHR*                                             accel;
		};

		/**
		* @brief AccelerationStructure Build Info and result.
		*/
		struct BuildAccelerationStructure
		{
			VkAccelerationStructureBuildGeometryInfoKHR     buildInfo{ VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR };
			VkAccelerationStructureBuildSizesInfoKHR        sizeInfo { VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR };
			const VkAccelerationStructureBuildRangeInfoKHR* rangeInfo;
			AccelKHR*                                       as;         // result acceleration structure
			AccelKHR                                        cleanupAS;
		};
		
	public:

		/**
		* @brief Constructor Function.
		* @param[in] vulkanState VulkanState.
		*/
		VulkanRayTracing(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanRayTracing() override = default;

		/**
		* @brief Get AccelerationStructure.
		* @return Returns AccelerationStructure.
		*/
		const VkAccelerationStructureKHR GetAccelerationStructure() const;

		/**
		* @brief Create all the BLAS from the vector of BlasInput.
		* There will be one BLAS per input-vector entry.
		* There will be as many BLAS as input.size().
		* The resulting BLAS (along with the inputs used to build) are stored in m_blas,
		* and can be referenced by index.
		* if flag has the 'Compact' flag, the BLAS will be compacted.
		* @param[in] input BlasInput.
		* @param[in] flags VkBuildAccelerationStructureFlagsKHR.
		*/
		void BuildBLAS(
			const std::vector<BlasInput>&        input , 
			VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR
		);

		/**
		* @brief Update part of BLAS.
		* @param[in] blasIdx index of BLAS.
		* @param[in] blas specific BlasInput.
		* @param[in] flags VkBuildAccelerationStructureFlagsKHR.
		*/
		void UpdateBlas(uint32_t blasIdx, const BlasInput& blas, VkBuildAccelerationStructureFlagsKHR flags) const;

		/******************************************TLAS*******************************************/

		/**
		* @brief Creating the top-level acceleration structure from the vector of Instance.
		* The resulting TLAS will be stored in m_tlas.
		* update is to rebuild the Tlas with updated matrices.
		* @param[in] instances Scene MeshPack Instances data.
		* @param[in] flags VkBuildAccelerationStructureFlagsKHR.
		* @param[in] update True if want update rather than create new one.
		*/
		void BuildTLAS(
			const std::vector<VkAccelerationStructureInstanceKHR>& instances   ,
			VkBuildAccelerationStructureFlagsKHR                   flags     = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			bool                                                   update    = false
		);

		/**
		* @brief Build TLAS from an array of VkAccelerationStructureInstanceKHR.
		* The resulting TLAS will be stored in m_tlas.
		* update is to rebuild the Tlas with updated matrices, flag must have the 'allow_update'
		* @param[in] instances Scene MeshPack Instances data.
		* @param[in] flags VkBuildAccelerationStructureFlagsKHR.
		* @param[in] update True if want update rather than create new one.
		* @param[in] motion True if  with VkAccelerationStructureMotionInstanceNV.
		*/
		template<class T>
		void BuildTLAS(
			const std::vector<T>&                instances   ,
			VkBuildAccelerationStructureFlagsKHR flags     = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
			bool                                 update    = false,
			bool                                 motion    = false
		);

		/**
		* @brief Low level of Tlas creation.
		* Creating the TLAS, called by buildTlas.
		* @param[in] cmdBuf VkCommandBuffer.
		* @param[in] countInstance number of instances.
		* @param[in] instBufferAddr Buffer address of instances.
		* @param[in] scratchBuffer ScratchBuffer Buffer.
		* @param[in] flags Build creation flag.
		* @param[in] update Update == animation.
		* @param[in] motion Motion Blur.
		*/
		void CmdCreateTLAS(
			VkCommandBuffer                      cmdBuf         ,
			uint32_t                             countInstance  ,
			VkDeviceAddress                      instBufferAddr ,
			std::unique_ptr<VulkanBuffer>&       scratchBuffer  ,
			VkBuildAccelerationStructureFlagsKHR flags          ,
			bool                                 update         ,
			bool                                 motion          
		);

		/*****************************************************************************************/

		/**********************************Hit Groups*********************************************/

		/**
		* @brief Set Scene hit groups.
		* @param[in] groups hit groups.
		*/
		void SetHitGroups(std::shared_ptr<std::unordered_map<std::string, uint32_t>> groups) { m_HitGroups = groups; }

		/**
		* @brief Get Scene hit groups.
		* @return Returns Scene hit groups.
		*/
		std::shared_ptr<std::unordered_map<std::string, uint32_t>> GetHitGroups();

		/*****************************************************************************************/

		/*****************************Shader Binding Table****************************************/

		/**
		* @brief Create Shader Binding Table.
		* @param[in] rgenCount Ray Generation shader count in raytracing material.
		* @param[in] missCount Ray Missing shader count in raytracing material.
		* @param[in] pipeline RayTracing Pipeline.
		*/
		void CreateRTShaderBindingTable(uint32_t rgenCount, uint32_t missCount, VkPipeline pipeline);

		/**
		* @brief Get SBT RgenRegion.
		* @return Returns SBT RgenRegion.
		*/
		VkStridedDeviceAddressRegionKHR& GetRgenRegion() { return m_RgenRegion; }

		/**
		* @brief Get SBT MissRegion.
		* @return Returns SBT MissRegion.
		*/
		VkStridedDeviceAddressRegionKHR& GetMissRegion() { return m_MissRegion; }

		/**
		* @brief Get SBT HitRegion.
		* @return Returns SBT HitRegion.
		*/
		VkStridedDeviceAddressRegionKHR& GetHitRegion() { return m_HitRegion; }

		/**
		* @brief Get SBT CallRegion.
		* @return Returns SBT CallRegion.
		*/
		VkStridedDeviceAddressRegionKHR& GetCallRegion() { return m_CallRegion; }

		/*****************************************************************************************/

		/**************************************Mesh Description***********************************/

		/**
		* @brief Get Mesh Description.
		* @return Returns Mesh Description.
		*/
		Attribute<uint64_t>& GetMeshDesc();

		/*****************************************************************************************/

	private:

		/**
		* @brief Creating the bottom level acceleration structure for all indices of `buildAs` vector.
		* The array of BuildAccelerationStructure was created in buildBlas and the vector of
		* indices limits the number of BLAS to create at once. This limits the amount of
		* memory needed when compacting the BLAS.
		* @param[in] cmdBuf VkCommandBuffer.
		* @param[in] indices BLAS indices.
		* @param[in] buildAs BuildAccelerationStructure.
		* @param[in] scratchAddress .
		* @param[in] queryPool Query AccelerationStructure data.
		*/
		void CmdCreateBLAS(
			VkCommandBuffer                          cmdBuf         ,
		    const std::vector<uint32_t>&             indices        ,
			std::vector<BuildAccelerationStructure>& buildAs        ,
			VkDeviceAddress                          scratchAddress ,
			std::shared_ptr<VulkanQueryPool>         queryPool
		) const;

		/**
		* @brief Create and replace a new acceleration structure and buffer based on the size retrieved by the Query.
		* We have to wait until all BLAS are built, to make a copy in the more suitable memory space.
		* This is the reason why we used m_cmdPool.submitAndWait(cmdBuf) before calling this function.
		* @param[in] cmdBuf VkCommandBuffer.
		* @param[in] indices BLAS indices.
		* @param[in] buildAs BuildAccelerationStructure.
		* @param[in] queryPool Query AccelerationStructure data.
		*/
		void CmdCompactBLAS(
			VkCommandBuffer                          cmdBuf   ,
			const std::vector<uint32_t>&             indices  ,
			std::vector<BuildAccelerationStructure>& buildAs  ,
			std::shared_ptr<VulkanQueryPool>         queryPool
		) const;

		/**
		* @brief Is item in flags.
		* @param[in] item VkFlags.
		* @param[in] flag VkFlags.
		* @return Returns true if item in flags.
		*/
		bool hasFlag(VkFlags item, VkFlags flag) { return (item & flag) == flag; }

		/**
		* @brief Create Acceleration.
		* @param[in] accel VkAccelerationStructureCreateInfoKHR.
		* @return Returns created Acceleration.
		*/
		AccelKHR CreateAcceleration(VkAccelerationStructureCreateInfoKHR& accel) const;

	private:

		/**
		* @brief Top-level acceleration structure.
		*/
		AccelKHR m_tlas;

		/**********************************Hit Groups*********************************************/

		/**
		* @brief Scene ray hit shader groups.
		*/
		std::shared_ptr<std::unordered_map<std::string, uint32_t>> m_HitGroups;

		/*****************************************************************************************/

		/*****************************Shader Binding Table****************************************/

		/**
		* @brief Shader Binding Table Buffer.
		*/
		std::unique_ptr<VulkanBuffer> m_RTSBTBuffer;

		/**
		* @brief Ray Generation Region.
		*/
		VkStridedDeviceAddressRegionKHR m_RgenRegion{};

		/**
		* @brief Ray Missing Region.
		*/
		VkStridedDeviceAddressRegionKHR m_MissRegion{};

		/**
		* @brief Ray Hit Region.
		*/
		VkStridedDeviceAddressRegionKHR m_HitRegion{};

		/**
		* @brief Ray Callable Region.
		*/
		VkStridedDeviceAddressRegionKHR m_CallRegion{};

		/*****************************************************************************************/

		/**************************************Mesh Description***********************************/

		/**
		* @brief Scene Mesh Description Buffer
		*/
		Attribute<uint64_t> m_MeshDesc;

		/*****************************************************************************************/
	};

	template<class T>
	inline void VulkanRayTracing::BuildTLAS(
		const std::vector<T>&                instances  , 
		VkBuildAccelerationStructureFlagsKHR flags      , 
		bool                                 update     , 
		bool                                 motion
	)
	{
		SPICES_PROFILE_ZONE;

		/**
		* @brief Cannot call buildTlas twice except to update.
		*/
		assert(m_tlas.accel == VK_NULL_HANDLE || update);
		const uint32_t countInstance = static_cast<uint32_t>(instances.size());

		/**
		* @brief Create a buffer holding the actual instance data (matrices++) for use by the AS builder.
		*/
		VulkanBuffer instancesBuffer(
			m_VulkanState                              ,
			"TLASInstancesBuffer"                      ,
			sizeof(T) * instances.size()               ,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT           |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT  | 
			VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR ,
			0
		);

		VulkanBuffer stagingBuffer(
			m_VulkanState                        ,
			"StagingBuffer"                      ,
			sizeof(T) * instances.size()         ,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT     ,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT  |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		stagingBuffer.WriteToBuffer(instances.data());

		instancesBuffer.CopyBuffer(stagingBuffer.Get(), instancesBuffer.Get(), sizeof(T) * instances.size());

		const VkDeviceAddress instBufferAddr = instancesBuffer.GetAddress();

		/**
		* @brief Command buffer to create the TLAS.
		*/
		std::unique_ptr<VulkanBuffer> scratchBuffer = nullptr;
		VulkanCommandBuffer::CustomGraphicCmd(m_VulkanState, [&](const VkCommandBuffer& commandBuffer) {

			/**
			* @brief Make sure the copy of the instance buffer are copied before triggering the acceleration structure build.
			*/
			VkMemoryBarrier                        barrier{};
			barrier.sType                        = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
			barrier.srcAccessMask                = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask                = VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;

			vkCmdPipelineBarrier(
				commandBuffer                                          ,
				VK_PIPELINE_STAGE_TRANSFER_BIT                         ,
				VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR ,
				0                                                      ,
				1                                                      ,
				&barrier                                               ,
				0                                                      ,
				nullptr                                                ,
				0                                                      ,
				nullptr
			);

			CmdCreateTLAS(
				commandBuffer  ,
				countInstance  , 
				instBufferAddr , 
				scratchBuffer  , 
				flags          , 
				update         ,
				motion
			);
		});
	}
}