/**
* @file VulkanTimelineSemaphore.h.
* @brief The VulkanTimelineSemaphore Class Definitions.
* @author Spices.
*/

#pragma once
#include "Core/Core.h"
#include "VulkanUtils.h"

namespace Spices {

	/**
	* @brief VulkanTimelineSemaphore Class.
	* This class is a wapper of timeline VkSemaphore.
	*/
	class VulkanTimelineSemaphore : public VulkanObject
	{
	public:

		/**
		* @brief Constructor Function.
		* @param[in] vulkanState The global VulkanState.
		*/
		VulkanTimelineSemaphore(VulkanState& vulkanState);

		/**
		* @brief Destructor Function.
		*/
		virtual ~VulkanTimelineSemaphore();

		/**
		* @brief Wait on this VkSemaphore using specific value.
		* @param[in] waitValue .
		*/
		void Wait(uint64_t waitValue = 1);

		/**
		* @brief Signal this VkSemaphore using specific value.
		* @param[in] signalValue .
		*/
		void Singal(uint64_t signalValue = 1);

		/**
		* @brief Get what Counter this VkSemaphore is now.
		* @return Returns Counter this VkSemaphore is now
		*/
		uint64_t GetValue();

	private:

		/**
		* @brief Timeline VkSemaphore.
		*/
		VkSemaphore m_TimelineSemaphore;
	};
}