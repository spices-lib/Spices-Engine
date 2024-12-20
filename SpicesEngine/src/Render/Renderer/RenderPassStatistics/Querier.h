/**
* @file Querier.h
* @brief The Querier Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Render/Vulkan/VulkanQueryPool.h"

namespace Spices {

	/**
	* @brief Basic interface of Queries.
	*/
	class Querier
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
			Max         = 2,           /* @brief Statistics Counts.      */
			ALL         = 0xFFFFFFFF,  /* @brief Statistics All items.   */
		};

		typedef uint32_t StatisticsFlags;

	public:

		/**
		* @brief Basic definition of result.
		*/
		struct Result
		{
			/**
			* @brief Constructor Function.
			*/
			Result() : valid(false) {}

			/**
			* @brief Destructor Function.
			*/
			virtual ~Result() = default;

			/**
			* @brief True if result is valid.
			*/
			bool valid;  

			/**
			* @brief Combine result with another Result.
			* @param[in] result another Result.
			*/
			virtual void Combine(Result* result) = 0;
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] type Querier Type.
		*/
		Querier(StatisticsBits type)
			: m_Type(type)
		{}

		/**
		* @brief Destructor Function.
		*/
		virtual ~Querier() = default;

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
		* @brief Store QueryPool Result.
		*/
		virtual void StorePoolResult() = 0;

		/**
		* @brief Get QueryPool Stored Result.
		*/
		std::shared_ptr<Querier::Result> GetPoolResult() const { return m_Result; }

		/**
		* @brief Get this Querier type.
		* @reutrn Returns Querier type.
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
		* @brief Specific Querier Type.
		*/
		StatisticsBits m_Type;
	};
}