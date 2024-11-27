/**
* @file PipelineStatisticsQueryer.cpp.
* @brief The PipelineStatisticsQueryer Class Implementation.
* @author Spices.
*/

#include "Pchheader.h"
#include "PipelineStatisticsQueryer.h"

namespace Spices {

	std::string PipelineStatisticEnumToString(PipelineStatisticEnum e)
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

	PipelineStatisticsQueryer::PipelineStatisticsQueryer(VulkanState& state)
		: Queryer(StatisticsBits::Pipeline)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool = std::make_unique<VulkanQueryPool>(state, VK_QUERY_TYPE_PIPELINE_STATISTICS, 1, (uint32_t)PipelineStatisticEnum::MAX * sizeof(uint64_t), (VkQueryPipelineStatisticFlags)PipelineStatisticEnum::ALL);
		m_Result    = std::make_shared<Result>();
	}

	void PipelineStatisticsQueryer::BeginQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->Reset(commandBuffer);
		m_QueryPool->BeginQuery(commandBuffer);
	}

	void PipelineStatisticsQueryer::EndQuery(VkCommandBuffer commandBuffer)
	{
		SPICES_PROFILE_ZONE;

		m_QueryPool->EndQuery(commandBuffer);
	}

	void PipelineStatisticsQueryer::StorePoolResult()
	{
		SPICES_PROFILE_ZONE;

		Result* result = static_cast<Result*>(m_Result.get());

		uint64_t poolResult[(int)PipelineStatisticEnum::MAX + 1];
		m_QueryPool->QueryResults(poolResult);

		//result->valid = poolResult[(int)PipelineStatisticEnum::MAX];
		result->valid = true;
		if (result->valid)
		{
			memcpy(result->statistics.data(), poolResult, sizeof(result->statistics));
		}
	}
}