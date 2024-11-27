/**
* @file TimestampQueryer.h
* @brief The TimestampQueryer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Queryer.h"

namespace Spices {

	/**
	* @brief Queryer of GPU Timestamp.
	*/
	class TimestampQueryer : public Queryer
	{
	public:

		/**
		* @brief Stored TimeStamp Result.
		*/
		struct Result : Queryer::Result
		{
			float timeStamp;  /* @brief TimeStamp in renderPass. */
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		explicit TimestampQueryer(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~TimestampQueryer() = default;

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