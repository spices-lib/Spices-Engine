/**
* @file Queryer.h
* @brief The Queryer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanQueryPool.h"

namespace Spices {

	/**
	* @brief Basic interface of Queries.
	*/
	class Queryer
	{
	public:

		/**
		* @brief Statistics types.
		*/
		enum StatisticsBits
		{
			None        = 0     ,      /* @brief None Statistics.        */
			Timestamp   = 1 << 0,      /* @brief Timestamp Statistics.   */
			Pipeline    = 1 << 1,      /* @brief Pipeline Statistics.    */
			Performance = 1 << 2,      /* @brief Performance Statistics. */
			Max         = 3,           /* @brief Statistics Counts.      */
			ALL         = 0xFFFFFFFF,  /* @brief Statistics All items.   */
		};

		typedef uint32_t StatisticsFlags;

	public:

		/**
		* @brief Basic definition of result.
		*/
		struct Result
		{
			bool valid;  /* @brief True if result is valid. */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] type Queryer Type.
		*/
		Queryer(StatisticsBits type)
			: m_Type(type)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Queryer() = default;

		/**
		* @brief Begin QueryPool.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		virtual void BeginQuery(VkCommandBuffer commandBuffer) = 0;

		/**
		* @brief End QueryPool.
		* @param[in] commandBuffer VkCommandBuffer.
		*/
		virtual void EndQuery(VkCommandBuffer commandBuffer) = 0;

		/**
		* @brief Get QueryPool Stored Result.
		*/
		virtual std::shared_ptr<Queryer::Result> GetPoolResult() = 0;

		/**
		* @brief Get this Queryer type.
		* @reutrn Returns Queryer type.
		*/
		StatisticsBits GetStatisticsType() const { return m_Type; }

	protected:

		/**
		* @brief QueryPool of RenderPass.
		*/
		std::unique_ptr<VulkanQueryPool> m_QueryPool;

		/**
		* @brief Result of QueryPool.
		*/
		std::shared_ptr<Result> m_Result;

		/**
		* @brief Specific Queryer Type.
		*/
		StatisticsBits m_Type;
	};
}