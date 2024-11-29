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
	static std::string PipelineStatisticEnumToString(PipelineStatisticEnum e)
	{
		switch (e)
		{
			case PipelineStatisticEnum::INPUT_ASSEMBLY_VERTICES:                     return "Input Assembly Vertices";
			case PipelineStatisticEnum::INPUT_ASSEMBLY_PRIMITIVES:                   return "Input Assembly Primitive";
			case PipelineStatisticEnum::VERTEX_SHADER_INVOCATIONS:                   return "Vertex Shader Invocations";
			case PipelineStatisticEnum::GEOMETRY_SHADER_INVOCATIONS:                 return "Geometry Shader Invocations";
			case PipelineStatisticEnum::GEOMETRY_SHADER_PRIMITIVES:                  return "Geometry Shader Primitive";
			case PipelineStatisticEnum::CLIPPING_INVOCATIONS:                        return "Clipping Invocations";
			case PipelineStatisticEnum::CLIPPING_PRIMITIVES:                         return "Clipping Primitive";
			case PipelineStatisticEnum::FRAGMENT_SHADER_INVOCATIONS:                 return "Fragment Shader Invocations";
			case PipelineStatisticEnum::TESSELLATION_CONTROL_SHADER_PATCHES:         return "Tessellation Control Invocations";
			case PipelineStatisticEnum::TESSELLATION_EVALUATION_SHADER_INVOCATIONS:  return "Tessellation Evaluation Invocations";
			case PipelineStatisticEnum::COMPUTE_SHADER_INVOCATIONS:                  return "Compute Shader Invocations";
			case PipelineStatisticEnum::TASK_SHADER_INVOCATIONS:                     return "Task Shader Invocations";
			case PipelineStatisticEnum::MESH_SHADER_INVOCATIONS:                     return "Mesh Shader Invocations";
			default:
			{
				SPICES_CORE_WARN("Special Enum Cannot be transfered inti a string.")
					return "";
			}
		}
	}

	/**
	* @brief Queryer of Pipeline.
	*/
	class PipelineStatisticsQueryer : public Queryer
	{
	public:

		/**
		* @brief Stored Pipeline Statistics Result.
		*/
		struct Result : public Queryer::Result
		{
			std::array<uint64_t, (size_t)PipelineStatisticEnum::MAX> statistics;

			/**
			* @brief Combine result with another Result.
			* @param[in] result another Result.
			*/
			virtual void Combine(Queryer::Result* result) override
			{
				auto r = static_cast<PipelineStatisticsQueryer::Result*>(result);

				if (!r->valid) return;

				for (int i = 0; i < (int)PipelineStatisticEnum::MAX; i++)
				{
					statistics[i] += r->statistics[i];
				}
			}
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
		* @brief Store QueryPool Result.
		*/
		virtual void StorePoolResult() override;
	};
}