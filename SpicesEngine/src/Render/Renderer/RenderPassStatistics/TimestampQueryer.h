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
		* @brief Get QueryPool Stored Result.
		*/
		virtual void GetPoolResult() override;

		/**
		* @brief Draw QueryPool Stored Result.
		*/
		virtual void DrawPoolResult() override;

	private:

		float m_TimeStamp;
	};
}