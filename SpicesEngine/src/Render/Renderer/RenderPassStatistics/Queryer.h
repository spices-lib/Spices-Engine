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
		* @brief Constructor Function.
		*/
		Queryer() = default;

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
		virtual void GetPoolResult() = 0;

	protected:

		/**
		* @brief QueryPool of Pipeline.
		*/
		std::unique_ptr<VulkanQueryPool> m_QueryPool;
	};
}