/**
* @file RenderPassStatistics.h
* @brief The RenderPassStatistics Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Queryer.h"

namespace Spices {

	/**
	* @brief Statistics of RenderPass(Renderer).
	*/
	class RenderPassStatistics
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		* @param[in] flags StatisticsFlags.
		*/
		RenderPassStatistics(VulkanState& state, Queryer::StatisticsFlags flags);
		
		/**
		* @brief Destructor Function.
		*/
		virtual ~RenderPassStatistics() = default;

		/**
		* @brief Begin Statistics.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void BeginStatistics(VkCommandBuffer commandBuffer);

		/**
		* @brief End Statistics.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		void EndStatistics(VkCommandBuffer commandBuffer);

		/**
		* @brief Iter all Statistics Result.
		* @param[in] fn Function pointer of how to do with statistics result.
		*/
		template<typename F>
		void IterStatisticsResult(F&& fn);

	private:
		
		/**
		* @brief Array of all statistics item.
		*/
		std::array<std::unique_ptr<Queryer>, (size_t)Queryer::Max> m_Queries;
	};

	template<typename F>
	inline void RenderPassStatistics::IterStatisticsResult(F&& fn)
	{
		SPICES_PROFILE_ZONE;

		for (auto& queryer : m_Queries)
		{
			if (queryer)
			{
				fn(queryer->GetStatisticsType(), queryer->GetPoolResult());
			}
		}
	}
}
