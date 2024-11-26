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
		* @brief Statistics types.
		*/
		enum StatisticsBits
		{
			None      = 0     ,    /* @brief None Statistics.      */
			Timestamp = 1 << 0,    /* @brief Timestamp Statistics. */
			Pipeline  = 1 << 1,    /* @brief Pipeline Statistics.  */
			Max       = 1 << 2,    /* @brief All Statistics.       */
		};

		typedef uint32_t StatisticsFlags;

	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		* @param[in] flags StatisticsFlags.
		*/
		RenderPassStatistics(VulkanState& state, StatisticsFlags flags);
		
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
		* @brief Get Statistics Result.
		*/
		void GetStatisticsResult();

		/**
		* @brief Draw Statistics Result.
		*/
		void DrawStatisticsResult();

	private:
		
		/**
		* @brief Array of all statistics item.
		*/
		std::array<std::unique_ptr<Queryer>, (size_t)StatisticsBits::Max> m_Queries;
	};
}
