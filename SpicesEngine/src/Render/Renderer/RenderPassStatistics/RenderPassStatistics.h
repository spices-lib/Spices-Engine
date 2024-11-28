/**
* @file RenderPassStatistics.h
* @brief The RenderPassStatistics Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Queryer.h"
#include "Core/Container/BehaveStateList.h"

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
		RenderPassStatistics(VulkanState& state, Queryer::StatisticsFlags flags);
		
		/**
		* @brief Destructor Function.
		*/
		virtual ~RenderPassStatistics() = default;

		/**
		* @brief Begin Statistics.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] flage StatisticsFlags.
		*/
		void BeginStatistics(VkCommandBuffer commandBuffer, Queryer::StatisticsFlags flage);

		/**
		* @brief End Statistics.
		* @param[in] commandBuffer VkCommandBuffer.
		* @param[in] flage StatisticsFlags.
		*/
		void EndStatistics(VkCommandBuffer commandBuffer, Queryer::StatisticsFlags flage);

		/**
		* @brief Store statistics result.
		*/
		void StoreStatistics();

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
