/**
* @file TimestampQuerier.h
* @brief The TimestampQuerier Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Querier.h"

namespace Spices {

	/**
	* @brief Querier of GPU Timestamp.
	*/
	class TimestampQuerier : public Querier
	{
	public:

		/**
		* @brief Stored TimeStamp Result.
		*/
		struct Result : public Querier::Result
		{
			float timeStamp;  /* @brief TimeStamp in renderPass. */

			/**
			* @brief Combine result with another Result.
			* @param[in] result another Result.
			*/
			virtual void Combine(Querier::Result* result) override
			{
				auto r = static_cast<TimestampQuerier::Result*>(result);

				if (!r->valid) return;

				timeStamp += r->timeStamp;
			}
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		explicit TimestampQuerier(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~TimestampQuerier() override = default;

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