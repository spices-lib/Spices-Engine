/**
* @file PipelineStatisticsQueryer.h
* @brief The PipelineStatisticsQueryer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Queryer.h"

namespace Spices {

	/**
	* @brief Enum of all pipeline statistics item. 
	*/
	enum class PipelineStatisticEnum
	{
		INPUT_ASSEMBLY_VERTICES                    = 0,          /* @brief Input Assembly of Vertics.                  */
		INPUT_ASSEMBLY_PRIMITIVES                  = 1,          /* @brief Input Assembly of Primitives.               */
		VERTEX_SHADER_INVOCATIONS                  = 2,          /* @brief Vertex Shader Invocations.                  */
		GEOMETRY_SHADER_INVOCATIONS                = 3,          /* @brief Geometry Shader Invocations.                */
		GEOMETRY_SHADER_PRIMITIVES                 = 4,          /* @brief Geometry Shader Primitives.                 */
		CLIPPING_INVOCATIONS                       = 5,          /* @brief Cliping Invocations.                        */
		CLIPPING_PRIMITIVES                        = 6,          /* @brief Cliping Primitives.                         */
		FRAGMENT_SHADER_INVOCATIONS                = 7,          /* @brief Fragment Shader Invocations.                */
		TESSELLATION_CONTROL_SHADER_PATCHES        = 8,          /* @brief Tessellation Control Shader Patches.        */
		TESSELLATION_EVALUATION_SHADER_INVOCATIONS = 9,          /* @brief Tessellation Evaluation Shader Invocations. */
		COMPUTE_SHADER_INVOCATIONS                 = 10,         /* @brief Compute Shader Invocations.                 */
		TASK_SHADER_INVOCATIONS                    = 11,         /* @brief Task Shader Invocations.                    */
		MESH_SHADER_INVOCATIONS                    = 12,         /* @brief Mesh Shader Invocations.                    */
		MAX                                        = 13,         /* @brief Statistics Counts.                          */
		ALL                                        = 0x00001FFF, /* @brief Statistics All items.                       */
	};

	/**
	* @brief Thransfer a PipelineStatisticEnum to a string.
	* @param[in] e PipelineStatisticEnum.
	* @reurn Returns transfered string.
	*/
	static std::string PipelineStatisticEnumToString(PipelineStatisticEnum e);

	/**
	* @brief Queryer of Pipeline.
	*/
	class PipelineStatisticsQueryer : public Queryer
	{
	public:

		/**
		* @brief Stored Pipeline Statistics Result.
		*/
		struct Result : Queryer::Result
		{
			std::array<uint64_t, (size_t)PipelineStatisticEnum::MAX> statistics;
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		explicit PipelineStatisticsQueryer(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~PipelineStatisticsQueryer() = default;

		/**
		* @brief Begin QueryPool.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		virtual void BeginQuery(VkCommandBuffer commandBuffer) override;

		/**
		* @brief End QueryPool.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		virtual void EndQuery(VkCommandBuffer commandBuffer) override;

		/**
		* @brief Get QueryPool Stored Result.
		*/
		virtual std::shared_ptr<Queryer::Result> GetPoolResult() override;
	};
}