/**
* @file RenderPassStatistics.h
* @brief The RenderPassStatistics Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Querier.h"

namespace Spices {

	class Renderer;

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
		RenderPassStatistics(VulkanState& state, Querier::StatisticsFlags flags);
		
		/**
		* @brief Destructor Function.
		*/
		virtual ~RenderPassStatistics() = default;

		/**
		* @brief Begin Statistics.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] flags StatisticsFlags.
		*/
		void BeginStatistics(VkCommandBuffer commandBuffer, Querier::StatisticsFlags flags) const;

		/**
		* @brief End Statistics.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] flags StatisticsFlags.
		*/
		void EndStatistics(VkCommandBuffer commandBuffer, Querier::StatisticsFlags flags) const;

		/**
		* @brief Store statistics result.
		*/
		void StoreStatistics() const;

		/**
		* @brief Iter all Statistics Result.
		* @param[in] flags StatisticsFlags.
		* @param[in] fn Function pointer of how to do with statistics result.
		*/
		template<typename F>
		void IterStatisticsResult(Querier::StatisticsFlags flags, F&& fn);

	private:
		
		/**
		* @brief Array of all statistics item.
		*/
		std::array<std::unique_ptr<Querier>, static_cast<size_t>(Querier::Max)> m_Queries;
	};

	template<typename F>
	inline void RenderPassStatistics::IterStatisticsResult(Querier::StatisticsFlags flage, F&& fn)
	{
		SPICES_PROFILE_ZONE;

		for (auto& querier : m_Queries)
		{
			if (!querier) continue;

			if (querier->GetStatisticsType() & flage)
			{
				fn(querier->GetStatisticsType(), querier->GetPoolResult());
			}
		}
	}

#ifdef SPICES_DEBUG

#define RENDERPASS_STATISTICS_ENDRENDERER(...)        { m_Renderer->m_StatisticsStateList->GetState()->ExecuteBehave("EndRenderer", __VA_ARGS__, VK_NULL_HANDLE); }
#define RENDERPASS_STATISTICS_ENDFRAME                { Renderer::m_StatisticsStateList->IncreateState(); }
#define RENDERPASS_STATISTICS_BEGINSTATISTICS(...)    { m_Renderer->m_StatisticsStateList->GetState()->ExecuteBehave("BeginStatistics", __VA_ARGS__); }
#define RENDERPASS_STATISTICS_ENDSTATISTICS(...)      { m_Renderer->m_StatisticsStateList->GetState()->ExecuteBehave("EndStatistics", __VA_ARGS__); }      
#define RENDERPASS_STATISTICS_CAPTUREFRAME            { Renderer::m_StatisticsStateList->ResetState(); }

#endif

#ifdef SPICES_RELEASE
 
#define RENDERPASS_STATISTICS_ENDRENDERER(...)
#define RENDERPASS_STATISTICS_ENDFRAME            
#define RENDERPASS_STATISTICS_BEGINSTATISTICS(...)
#define RENDERPASS_STATISTICS_ENDSTATISTICS(...)  
#define RENDERPASS_STATISTICS_CAPTUREFRAME  

#endif
}
