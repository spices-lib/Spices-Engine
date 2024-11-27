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

	public:

		/**
		* @brief Whether capture all next frame RenderPass.
		*/
		static bool m_IsCaptureNextFrame;

		/**
		* @brief Whether capture all this frame RenderPass.
		*/
		static bool m_IsCaptureThisFrame;

		/**
		* @brief Whether Store result all next frame RenderPass.
		*/
		static bool m_IsStoreResultNextFrame;
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

#define RENDERPASS_STATISTICS_BEGINFRAME              { RenderPassStatistics::m_IsCaptureThisFrame = RenderPassStatistics::m_IsCaptureNextFrame; }
#define RENDERPASS_STATISTICS_ENDRENDERER             { if(RenderPassStatistics::m_IsStoreResultNextFrame) ThreadPool::Get()->SubmitPoolTask([&](std::shared_ptr<RendererSubPass> subPass){ subPass->StoreStatistics(); }, GetStatisticsRendererPass()); }
#define RENDERPASS_STATISTICS_ENDFRAME                { RenderPassStatistics::m_IsStoreResultNextFrame = false; if(RenderPassStatistics::m_IsCaptureThisFrame) { RenderPassStatistics::m_IsCaptureNextFrame = false; RenderPassStatistics::m_IsStoreResultNextFrame = true; } }
#define RENDERPASS_STATISTICS_BEGINSTATISTICS(...)    { if(RenderPassStatistics::m_IsCaptureThisFrame) GetStatisticsRendererPass()->BeginStatistics(__VA_ARGS__); }
#define RENDERPASS_STATISTICS_ENDSTATISTICS(...)      { if(RenderPassStatistics::m_IsCaptureThisFrame) GetStatisticsRendererPass()->EndStatistics(__VA_ARGS__);   }
#define RENDERPASS_STATISTICS_CAPTUREFRAME            { RenderPassStatistics::m_IsCaptureNextFrame = true; }           

#endif

#ifdef SPICES_RELEASE

#define RENDERPASS_STATISTICS_BEGINFRAME   
#define RENDERPASS_STATISTICS_ENDRENDERER
#define RENDERPASS_STATISTICS_ENDFRAME            
#define RENDERPASS_STATISTICS_BEGINSTATISTICS(...)
#define RENDERPASS_STATISTICS_ENDSTATISTICS(...)  
#define RENDERPASS_STATISTICS_CAPTUREFRAME  

#endif
}
