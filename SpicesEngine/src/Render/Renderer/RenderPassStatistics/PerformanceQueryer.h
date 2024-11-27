/**
* @file PerformanceQueryer.h
* @brief The PerformanceQueryer Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "Queryer.h"

namespace Spices {

	/**
	* @brief Queryer of Performance.
	*/
	class PerformanceQueryer : public Queryer
	{
	public:

		/**
		* @brief Stored Pipeline Statistics Result.
		*/
		struct Result : Queryer::Result
		{
			
		};

	public:

		/**
		* @brief Constructor Function.
		* @param[in] state VulkanState.
		*/
		explicit PerformanceQueryer(VulkanState& state);

		/**
		* @brief Destructor Function.
		*/
		virtual ~PerformanceQueryer() = default;

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
		virtual std::shared_ptr<Queryer::Result> GetPoolResult() override;
	};
}