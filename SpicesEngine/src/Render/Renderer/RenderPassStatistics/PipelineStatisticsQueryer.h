#pragma once
#include "Core/Core.h"
#include "Queryer.h"

namespace Spices {

	/**
	* @brief Enum of all pipeline statistics item. 
	*/
	enum class PipelineStatisticEnum
	{
		INPUT_ASSEMBLY_VERTICES                    = 0,     /* @brief Input Assembly of Vertics.                  */
		INPUT_ASSEMBLY_PRIMITIVES                  = 1,     /* @brief Input Assembly of Primitives.               */
		VERTEX_SHADER_INVOCATIONS                  = 2,     /* @brief Vertex Shader Invocations.                  */
		GEOMETRY_SHADER_INVOCATIONS                = 3,     /* @brief Geometry Shader Invocations.                */
		GEOMETRY_SHADER_PRIMITIVES                 = 4,     /* @brief Geometry Shader Primitives.                 */
		CLIPPING_INVOCATIONS                       = 5,     /* @brief Cliping Invocations.                        */
		CLIPPING_PRIMITIVES                        = 6,     /* @brief Cliping Primitives.                         */
		FRAGMENT_SHADER_INVOCATIONS                = 7,     /* @brief Fragment Shader Invocations.                */
		TESSELLATION_CONTROL_SHADER_PATCHES        = 8,     /* @brief Tessellation Control Shader Patches.        */
		TESSELLATION_EVALUATION_SHADER_INVOCATIONS = 9,     /* @brief Tessellation Evaluation Shader Invocations. */
		COMPUTE_SHADER_INVOCATIONS                 = 10,    /* @brief Compute Shader Invocations.                 */
		TASK_SHADER_INVOCATIONS                    = 11,    /* @brief Task Shader Invocations.                    */
		MESH_SHADER_INVOCATIONS                    = 12,    /* @brief Mesh Shader Invocations.                    */
		CLUSTER_CULLING_SHADER_INVOCATIONS         = 13,    /* @brief Cluster Culling Shader Invocations.         */
		MAX                                        = 14,
	};

	class PipelineStatisticsQueryer : public Queryer
	{
	public:

		explicit PipelineStatisticsQueryer(VulkanState& state);
		virtual ~PipelineStatisticsQueryer() = default;

		virtual void BeginQuery(VkCommandBuffer commandBuffer) override;
		virtual void EndQuery(VkCommandBuffer commandBuffer) override;

		virtual void GetPoolResult() override;

	private:

		std::array<std::unique_ptr<VulkanQueryPool>, (size_t)PipelineStatisticEnum::MAX> m_QueryPool;
	};
}